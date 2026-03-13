#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <typeindex>
#include <sstream>
#include <tuple>
#include <utility>
#include <stdexcept>
#include <type_traits>
#include <cxxabi.h>


namespace common{
    class DIContainer { 
    private:
        // 核心1 创建器 -> 封装“实例创建逻辑” 区分单例/原型
        using InstanceCreator = std::function<std::shared_ptr<void>()>;     /* InstanceCreator是std::function封装的类型 */
        std::unordered_map<std::type_index, InstanceCreator> _creators;
    
        // 存储：类型标识 → 单例实例（shared_ptr 管理生命周期）复用已创建的单例实例
        std::unordered_map<std::type_index, std::shared_ptr<void>> _singletonCache;     /* 键是std::type_info的封装类型 */

        DIContainer() = default;  /*私有构造，确保单例模式获取*/
        DIContainer(const DIContainer&) = delete;  /*禁用拷贝构造*/
        DIContainer& operator=(const DIContainer&) = delete;

    public:
        /* 单例模式：全局唯一实例 */
        static DIContainer& getInstance() {
            static DIContainer instance;
            return instance;
        }

        /* 注册接口1: 单例+工厂（适配DBConnection全局唯一连接） */
        template <typename T>
        void registerSigletonByFactory(std::function<std::shared_ptr<T>()> factoryFunc) {
            const std::type_index typeIdx = typeid(T);
            if(_creators.find(typeIdx) != _creators.end()) {
                throw std::runtime_error(fmtError("重复注册工厂模式单例", typeid(T).name()));
            }

            // 注册工厂创建逻辑：首次调用工厂函数，后续复用缓存
            _creators[typeIdx] = [this, factoryFunc]() -> std::shared_ptr<void> {
                const auto idx = typeid(T);
                if(_singletonCache.find(idx) == _singletonCache.end()) {
                    _singletonCache[idx] = factoryFunc();  // 调用简单工厂模式
                }
                return _singletonCache[idx];
            };
        }

        /* 2.原型 + 工厂函数（极少数用，仅特殊场景，每次创建新的DB连接 */
        template <typename T>
        void registerPrototypeByFactory(std::function<std::shared_ptr<T>()> factoryFunc){
            const std::type_index typeIdx = typeid(T);
            if(_creators.find(typeIdx) != _creators.end()){
                throw std::runtime_error(fmtError("重复注册工厂模式原型", typeid(T).name()));
            }
            // 每次调用都执行工厂函数创建新实例
            _creators[typeIdx] = [factoryFunc]() -> std::shared_ptr<void> {
                return factoryFunc();
            };
        }

        // 原有接口：保留所有功能
        template <typename T, typename Impl, typename... Args>
        void registerSigleton(Args&&... args) {
            const std::type_index typeIdx = typeid(T);

            if (_creators.find(typeIdx) != _creators.end()) {
                throw std::runtime_error(fmtError("重复注册单例", typeid(T).name()));
            }
            auto tuple = std::make_tuple(std::forward<Args>(args)...);

            _creators[typeIdx] = [this, tuple = std::move(tuple)]() mutable -> std::shared_ptr<void> {
                const std::type_index idx = typeid(T);
                if(_singletonCache.find(idx) == _singletonCache.end()) {
                    _singletonCache[idx] = createInstance<Impl>(std::move(tuple));
                }
                return _singletonCache[idx];
            };
        }
        template <typename T, typename... Args>
        void registerSigleton(Args&&... args) {
            registerSigleton<T, T>(std::forward<Args>(args)...);
        }

        template <typename T>
        void registerSingleton(std::shared_ptr<T> instance) {
            const std::type_index typeIdx = typeid(T);
            if (_creators.find(typeIdx) != _creators.end()) {
                throw std::runtime_error(fmtError("重复注册单例", typeid(T).name()));
            }
            _creators[typeIdx] = [instance]()->std::shared_ptr<void> {return instance;};
            _singletonCache[typeIdx] = instance;
        }

        template <typename T, typename Impl, typename... Args>
        void registerPrototype(Args&&... args) {
            const std::type_index typeIdx = typeid(T);
            if (_creators.find(typeIdx) != _creators.end()) {
                throw std::runtime_error(fmtError("重复注册原型", typeid(T).name()));
            }
             auto tuple = std::make_tuple(std::forward<Args>(args)...); // 外部创建参数元组

            _creators[typeIdx] = [this, tuple]()->std::shared_ptr<void> {   // 添加this捕捉
                return createInstance<Impl>(tuple);
            };
        }

        template <typename T>
        std::shared_ptr<T> resolve() {
            const std::type_index typeIdx = typeid(T);
            if ( _creators.find(typeIdx) == _creators.end()) {
                throw std::runtime_error(fmtError("依赖未注册", typeid(T).name()));
            }
            std::shared_ptr<void> rawInstance = _creators[typeIdx]();
            return std::static_pointer_cast<T>(rawInstance);
        }
    private:
        std::string fmtError(const std::string& msg, const char* typeName) {
            std::ostringstream oss;
            oss << msg << ": " << demangle(typeName);
            return oss.str();
        }

        std::string demangle( const char* mangledName) {
        #if defined(__GNUG__) || defined(__clang__)
            int status = 0;
            char* demangled = __cxxabiv1::__cxa_demangle(mangledName, nullptr, nullptr, &status);
            std::string result = (status == 0) ? demangled : mangledName;
            free(demangled);
            return result;
        #elif defined(_MSC_VER)
            return std::string(mangledName);
        #else
            return std::string(mangledName);
        #endif
        }

        template <typename Impl, typename Tuple, std::size_t... Idx>
        std::shared_ptr<Impl> createInstanceImpl(Tuple&& tuple, std::index_sequence<Idx...>) {
            using RawTuple = std::decay_t<Tuple>;
            // return std::make_shared<Impl>(resolve<std::tuple_element_t<Idx, RawTuple>>()...);
            return std::make_shared<Impl>(std::get<Idx>(std::forward<Tuple>(tuple))...);    // 直接传入实例
        }

        template <typename Impl, typename Tuple>
        std::shared_ptr<Impl> createInstance(Tuple&& tuple) {
            constexpr std::size_t argCount = std::tuple_size<std::remove_reference_t<Tuple>>::value;
            return createInstanceImpl<Impl>(std::forward<Tuple>(tuple), std::make_index_sequence<argCount>{});
        }

        template <typename Impl>
        std::shared_ptr<Impl> createInstance(std::tuple<>) {
            return std::make_shared<Impl>();
        }
    };
}