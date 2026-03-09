结合你的场景（十几个 Handler 需注册、代码量 10~50k 行），推荐采用「轻量级自定义 DI 容器 + 智能指针（shared_ptr） + Handler 工厂」的组合方案。
这个方案的核心是：保留 “构造函数传参” 的测试友好性，同时用 DI 容器解决 “依赖链冗长、扩展性差” 的问题，既适配 10~50k 行的中型项目规模，又能支撑后续十几个 Handler 的扩展，且学习 / 开发成本远低于重型 DI 框架（如 Boost.DI）。
一、方案核心设计思路
模块	设计方式
核心依赖（DB/Service）	用 shared_ptr 管理生命周期，注册到 DI 容器中（单例模式），避免重复创建；
Handler 创建	每个 Handler 保留 “构造传参”（保证测试友好），但依赖由 DI 容器自动解析；
Handler 注册	封装工厂方法，从 DI 容器获取依赖并创建 Handler，统一注册到 CommandRouter；
扩展性	新增 Handler / 依赖时，仅需在容器中注册，无需修改所有调用处；

三、方案核心优势（适配你的场景）
1. 解决 “十几个 Handler 注册冗余” 的问题
新增 Handler 时，仅需：
① 定义 Handler 类（保留构造传参）；
② 在 HandlerFactory 中添加 createXXXHandler 方法；
③ 在 main 中加一行 registerHandler；
无需重复编写依赖链（DB→DAO→Service→Handler），所有依赖由 DI 容器自动解析。
2. 保留 “构造传参” 的测试友好性
单元测试时，可绕过 DI 容器，直接传 Mock 对象：
cpp
运行
// 测试 UserRegisterHandler
#include "UserRegisterHandler.h"
#include "gmock/gmock.h"

// Mock UserService
class MockUserService : public UserService {
public:
    MOCK_METHOD(void, registerUser, (const std::string&), (override));
};

TEST(UserRegisterHandlerTest, Handle) {
    auto mockService = std::make_shared<MockUserService>(nullptr);
    auto handler = std::make_shared<UserRegisterHandler>(mockService);
    EXPECT_CALL(*mockService, registerUser("test_user")).Times(1);
    handler->handle();
}
3. 生命周期安全（无悬空引用）
所有核心依赖用 shared_ptr 管理，DI 容器保证单例生命周期与程序一致；
Handler 持有 shared_ptr，即使容器销毁，只要 Handler 存在，依赖就不会释放，避免悬空引用。
4. 扩展性适配 10~50k 行代码
新增依赖（如 LogService）：仅需在 DI 容器中注册 LogService，修改依赖它的 Service 构造函数，其他代码无需改动；
新增 Handler：仅需添加 Handler 类 + 工厂方法 + 注册行，符合 “开闭原则”。
四、落地注意事项
线程安全：DI 容器的 registerSingleton 需在程序启动时（主线程）完成，后续仅调用 resolve（只读，线程安全）；
懒加载优化：若 Handler 数量多，可将 Handler 创建逻辑改为 “首次调用时初始化”，避免启动时创建所有 Handler；
依赖顺序：注册依赖时必须按 “底层→上层” 顺序（如先 DB，再 DAO，最后 Service），否则会解析失败；
避免过度设计：无需引入 Boost.DI 等重型框架，自定义轻量容器足够支撑 50k 行代码规模。
五、为什么不推荐其他方案？
方案	不推荐原因
纯构造传参	十几个 Handler 会导致依赖链代码重复，新增 / 修改依赖需改多处，维护成本高；
单例模式	测试时无法 Mock 依赖，多实例场景（如多 DB 连接）不支持，耦合度高；
全局上下文	依赖隐藏在上下文内部，可读性差，测试时替换依赖不如 DI 容器灵活；
重型 DI 容器	学习成本高，10~50k 行代码无需引入复杂框架，反而增加维护成本；
这个方案是 10~50k 行中型项目的 “黄金平衡点”：既解决了纯构造传参的冗余问题，又保留了测试友好性，同时支撑后续十几个 Handler 的扩展，且代码结构清晰、易维护。