// DIContainer.h（通用模块，后续无需修改）
#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>
#include <utility>

class DIContainer {
private:
    // 存储：类型标识 → 单例实例（shared_ptr 管理生命周期）
    std::unordered_map<std::type_index, std::shared_ptr<void>> _singletons;

public:
    // 注册单例依赖（核心：全局仅一份，所有 Handler 复用）
    template <typename T>
    void registerSingleton(std::shared_ptr<T> instance) {
        _singletons[typeid(T)] = instance;
    }

    // 注册单例依赖（简化版：自动创建实例，适用于无参/已知参数的依赖）
    template <typename T, typename... Args>
    void registerSingleton(Args&&... args) {
        registerSingleton(std::make_shared<T>(std::forward<Args>(args)...));
    }

    // 解析依赖（从容器中获取已注册的实例）
    template <typename T>
    std::shared_ptr<T> resolve() {
        auto it = _singletons.find(typeid(T));
        if (it == _singletons.end()) {
            throw std::runtime_error("依赖未注册：" + std::string(typeid(T).name()));
        }
        return std::static_pointer_cast<T>(it->second);
    }

    // 禁用拷贝（保证容器全局唯一）
    DIContainer(const DIContainer&) = delete;
    DIContainer& operator=(const DIContainer&) = delete;
    // 全局唯一容器实例（C++11 线程安全）
    static DIContainer& getInstance() {
        static DIContainer instance;
        return instance;
    }

private:
    // 私有构造，仅能通过 getInstance 获取
    DIContainer() = default;
};

// 简化调用的宏（可选，减少重复代码）
#define DI_REGISTER_SINGLETON(T, ...) DIContainer::getInstance().registerSingleton<T>(__VA_ARGS__)
#define DI_RESOLVE(T) DIContainer::getInstance().resolve<T>()