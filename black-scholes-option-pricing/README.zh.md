# Black-Scholes 期权定价模型与套利策略

> [English](README.md) | **中文**

> 北京交通大学计算机学院本科毕业设计 (2015 年 6 月)
> 作者：郭珣 (11281004)
> 指导教师：刘一

将 Black-Scholes 期权定价模型与基于隐含波动率的套利策略用 C++ 与 Qt 实现，并以恒生指数 1986-12-31 至 2013-06-28 的真实交易日收盘价作为样本数据，使用 Eviews + GARCH(1,1) 估计波动率，最终在 Qt GUI 中支持期权定价、隐含波动率反解、套利组合收益评估等功能。

---

## 1. 研究背景

期权是金融衍生品中最重要的品种之一，其合理定价问题是金融数学的核心方向。**Black-Scholes 模型**（Black, Scholes, Merton, 1973）通过两大数学基础——**Wiener 过程**与 **Ito 积分**——推导出无红利欧式期权的解析定价公式，使期权定价首次具备了可计算性，是 1997 年诺贝尔经济学奖的主要工作。

本项目以工程实现的角度回答三个问题：
1. 如何用程序实现 Black-Scholes 解析公式？
2. 如何从历史价格估计模型最关键的参数——**波动率 σ**？
3. 在估出的隐含波动率下，构建哪些套利组合，能在何种市场环境下盈利？

## 2. 理论基础

### 2.1 Wiener 过程
描述 Brown 运动的连续随机过程 W(t)，满足 W(0)=0、增量服从正态分布 N(0,t)。直接的 W(t) 可能为负，因此对股价采用**指数化处理**（带漂移的指数布朗运动）：

```
dS = μS dt + σS dW
```

### 2.2 Ito 积分
对 W(t) 直接套用牛顿微分会丢失二阶项，Ito (1951) 给出了正确的随机微分公式，是 BS 微分方程推导的关键。

### 2.3 Black-Scholes 公式

在七条假设（指数布朗运动、无套利、无交易费用、连续交易、常数无风险利率等）下，得到欧式看涨/看跌期权解析定价：

```
Call: C = S·N(d1) − K·e^(-rT)·N(d2)
Put : P = K·e^(-rT)·N(-d2) − S·N(-d1)

d1 = [ln(S/K) + (r + σ²/2)T] / (σ√T)
d2 = d1 − σ√T
```

### 2.4 Greeks（敏感性参数）
项目实现了 Δ / Γ / Θ / Vega / ρ 五个 partial derivative，用于风险刻画与对冲分析。

## 3. 数据与波动率估计

| 项目 | 内容 |
|-----|-----|
| 样本 | 恒生指数 1986-12-31 ~ 2013-06-28 收盘价（约 6500 个交易日） |
| 处理工具 | Eviews（时间序列建模）+ C++（计算/可视化） |
| 波动率模型 | GARCH(1,1)：σ²ₜ = α + β·ε²ₜ₋₁ + γ·σ²ₜ₋₁ |
| 长期方差 | σ̄² = α / (1 − β − γ) |
| 年化 | σ_annual = √(σ̄² · 242) |

项目同时实现：
- **历史波动率**：基于对数收益率的样本标准差
- **隐含波动率**：用 Newton-Raphson 与二分法两种数值方法从市场期权价反解 σ

## 4. 套利策略

| 策略 | 组合 | 适用场景 |
|-----|-----|---------|
| **蝶式 Butterfly** | 买入一高一低 Call + 卖出 2 份中间价 Call | 预期波动率小幅下降 |
| **鹰式 Hawk/Condor** | 买 2 份外侧 Call + 卖 2 份内侧 Call | 类似蝶式但容忍小幅波动 |
| **跨式 Straddle** | 同执行价同时买 Call + Put | 预期价格大幅变动（方向未知） |
| **宽跨式 Strangle** | 不同执行价 Call + Put | 跨式的低成本变体 |

每种策略都封装为 `ArbitStrategy` 基类的子类，实现 `ArbitENow()` 计算当下损益。

## 5. 系统实现

### 技术栈
- **语言**：C++
- **GUI**：Qt 4.x（`QMainWindow` + `.ui` 设计器）
- **依赖**：标准 C++ 库 + Qt（无第三方金融库）

### 目录结构

```
black-scholes-option-pricing/
├── README.md                    # 本文件
├── docs/
│   └── thesis-outline.md        # 论文章节梳理
└── code/
    ├── core/                    # 核心算法（命令行可独立运行）
    │   ├── PriceControl.h/.cpp  # BlackScholes 类 + 套利策略类
    │   ├── BlackScholes.cpp     # 主流程：批量套利回测
    │   └── UnitTest.h/.cpp      # 公式正确性测试
    └── qt-app/                  # Qt GUI 程序（BlackScholes_01）
        ├── BlackScholes_01.pro
        ├── main.cpp
        ├── mainwindow.{h,cpp,ui}
        ├── optionprice.*        # 期权定价对话框
        ├── implivola.*          # 隐含波动率
        ├── garchwidget.*        # GARCH 波动率估计
        ├── hedgepara.*          # Greeks
        ├── arbitresult.*        # 套利结果
        ├── curveplotwidget.*    # 收益曲线绘制
        ├── LineGraphWidget.*    # 通用折线图组件
        └── 1986-2013.txt        # 恒生指数样本数据
```

### 核心类关系

```
BlackScholes
├── normalDistribution(u)              # N(·)，分段近似实现
├── option_price_call/put_BlackScholes # 解析定价
├── option_price_implied_volatility_*  # Newton + Bisection 反解
├── option_price_partials_call/put     # Greeks
└── getSigma(α, β, γ, t)               # GARCH 长期波动率

ArbitStrategy (抽象基类)
├── BufferFly   (n=3，Butterfly)
├── Hawk        (n=4，Condor)
└── Straddle    (n=2，Straddle / Strangle 共用)

PriceManager
└── 加载 1986-2013.txt，按序号查询日期/收盘价/执行价
```

### Qt GUI 主要功能页

通过菜单进入对应窗口：
- 期权定价 (`optionprice`)
- 隐含波动率反解 (`implivola`)
- GARCH 波动率估计 (`garchwidget`)
- Greeks 计算与对冲参数 (`hedgepara`)
- 套利策略组合及收益曲线 (`arbitresult` + `curveplotwidget`)

## 6. 主要实验结论

- **蝶式 / 鹰式**：在波动率平稳期能稳定获利，鹰式对小幅波动容忍度更高但单次收益偏低。
- **跨式 / 宽跨式**：恒生指数样本期内波动率整体偏稳，跨式套利收益不显著；宽跨式因价差降低成本，相对更优。
- **波动率估计**：GARCH(1,1) 估出的波动率与市场长期值吻合较好，作为定价输入可接受。
- **隐含波动率**：Newton 法收敛快但需良好初值；二分法稳定但慢，工程上两者互补使用。

## 7. 编译与运行

### 核心命令行版本

```bash
cd code/core
# 需在 Visual Studio 或 g++ 下编译（原项目使用 stdafx.h，可去除该 include）
g++ -std=c++11 BlackScholes.cpp PriceControl.cpp UnitTest.cpp -o bs
./bs
```

### Qt GUI 版本

```bash
cd code/qt-app
qmake BlackScholes_01.pro
make
./BlackScholes_01
```

> 注：原项目基于 Qt 4.x 与 MSVC，迁移到 Qt 5/6 需替换 `QtGui/QApplication` 为 `QtWidgets/QApplication`，并去除 `QTextCodec::setCodecForTr` 等已废弃 API。

## 8. 参考与延伸

- Black, F. & Scholes, M. (1973). *The Pricing of Options and Corporate Liabilities*. Journal of Political Economy.
- Bollerslev, T. (1986). *Generalized autoregressive conditional heteroskedasticity*. Journal of Econometrics.
- Hull, J. C. *Options, Futures, and Other Derivatives* (经典教材，BS 推导 / Greeks / 套利策略均有覆盖)。

---

*本目录是 2015 年本科毕业设计的代码与思路梳理，原始论文 docx、附件实验数据和开题报告未上传到本仓库（含敏感日期与个人信息），仅同步了核心源代码与方法学梳理。*
