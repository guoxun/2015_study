# Black-Scholes Option Pricing Model and Arbitrage Strategies

> **English** | [中文](README.zh.md)

> Undergraduate thesis project, School of Computer and Information Technology, Beijing Jiaotong University (June 2015)
> Author: Guo Xun (Student ID: 11281004)
> Supervisor: Prof. Liu Yi

A C++ / Qt implementation of the Black-Scholes option pricing model and implied-volatility-based arbitrage strategies. Uses Hang Seng Index daily closing prices from 1986-12-31 to 2013-06-28 as the sample dataset, with volatility estimated via Eviews + GARCH(1,1). The Qt GUI supports option pricing, implied volatility inversion, and arbitrage portfolio P&L evaluation.

---

## 1. Background

Options are among the most important derivatives in modern finance, and the rational pricing of options has been a central topic in financial mathematics. The **Black-Scholes model** (Black, Scholes, Merton, 1973) — built on **Wiener processes** and **Ito integrals** — yields a closed-form pricing formula for European options without dividends, making option pricing computable for the first time. The work was awarded the 1997 Nobel Prize in Economics.

This project addresses three engineering questions:
1. How to implement the Black-Scholes analytical formula in code?
2. How to estimate the model's most critical parameter — **volatility σ** — from historical prices?
3. Given the inferred implied volatility, which arbitrage portfolios profit under which market regimes?

## 2. Theoretical Foundation

### 2.1 Wiener Process
A continuous stochastic process W(t) satisfying W(0)=0 with increments distributed as N(0, t). Since W(t) can be negative, stock prices use an **exponentiated** form (geometric Brownian motion with drift):

```
dS = μS dt + σS dW
```

### 2.2 Ito Integral
Naive Newtonian differentiation of W(t) drops the second-order term. Ito (1951) provided the correct stochastic differential rule, which is essential to deriving the BS PDE.

### 2.3 Black-Scholes Formula

Under seven assumptions (geometric Brownian motion, no arbitrage, no transaction costs, continuous trading, constant risk-free rate, etc.), the closed-form European call/put pricing formulas are:

```
Call: C = S·N(d1) − K·e^(-rT)·N(d2)
Put : P = K·e^(-rT)·N(-d2) − S·N(-d1)

d1 = [ln(S/K) + (r + σ²/2)T] / (σ√T)
d2 = d1 − σ√T
```

### 2.4 Greeks (Sensitivity Parameters)
The project implements Δ / Γ / Θ / Vega / ρ as partial derivatives of the BS formula, used for risk characterization and hedging analysis.

## 3. Data and Volatility Estimation

| Item | Value |
|-----|-----|
| Sample | Hang Seng Index closing prices, 1986-12-31 ~ 2013-06-28 (~6,500 trading days) |
| Tools | Eviews (time-series modeling) + C++ (computation / visualization) |
| Volatility model | GARCH(1,1): σ²ₜ = α + β·ε²ₜ₋₁ + γ·σ²ₜ₋₁ |
| Long-run variance | σ̄² = α / (1 − β − γ) |
| Annualization | σ_annual = √(σ̄² · 242) |

The project also implements:
- **Historical volatility** based on log-return sample standard deviation
- **Implied volatility** inversion via both Newton-Raphson and bisection from market option prices

## 4. Arbitrage Strategies

| Strategy | Composition | Best Suited For |
|---------|-------------|-----------------|
| **Butterfly** | Buy 1 high-strike + 1 low-strike Call, sell 2 mid-strike Calls | Slight decline in volatility |
| **Hawk / Condor** | Buy 2 outer Calls, sell 2 inner Calls | Similar to butterfly but tolerates small fluctuations |
| **Straddle** | Buy a Call and a Put at the same strike | Large price move expected (direction unknown) |
| **Strangle** | Call and Put at different strikes | Lower-cost variant of straddle |

Each strategy is implemented as a subclass of the abstract `ArbitStrategy` base class, with `ArbitENow()` computing immediate P&L.

## 5. System Implementation

### Tech Stack
- **Language**: C++
- **GUI**: Qt 4.x (`QMainWindow` + `.ui` Designer files)
- **Dependencies**: C++ standard library + Qt only (no third-party financial libraries)

### Directory Layout

```
black-scholes-option-pricing/
├── README.md                    # this file (English)
├── README.zh.md                 # Chinese version
├── docs/
│   └── thesis-outline.md        # thesis chapter breakdown
└── code/
    ├── core/                    # core algorithms (CLI standalone)
    │   ├── PriceControl.h/.cpp  # BlackScholes class + arbitrage classes
    │   ├── BlackScholes.cpp     # main flow: batch arbitrage backtest
    │   └── UnitTest.h/.cpp      # formula correctness tests
    └── qt-app/                  # Qt GUI program (BlackScholes_01)
        ├── BlackScholes_01.pro
        ├── main.cpp
        ├── mainwindow.{h,cpp,ui}
        ├── optionprice.*        # option pricing dialog
        ├── implivola.*          # implied volatility
        ├── garchwidget.*        # GARCH volatility estimation
        ├── hedgepara.*          # Greeks
        ├── arbitresult.*        # arbitrage result
        ├── curveplotwidget.*    # P&L curve plotting
        ├── LineGraphWidget.*    # generic line-chart widget
        └── 1986-2013.txt        # Hang Seng Index sample data
```

### Class Relationships

```
BlackScholes
├── normalDistribution(u)              # N(·), piecewise approximation
├── option_price_call/put_BlackScholes # analytical pricing
├── option_price_implied_volatility_*  # Newton + Bisection inversion
├── option_price_partials_call/put     # Greeks
└── getSigma(α, β, γ, t)               # GARCH long-run volatility

ArbitStrategy (abstract base)
├── BufferFly   (n=3, Butterfly)
├── Hawk        (n=4, Condor)
└── Straddle    (n=2, used for both Straddle and Strangle)

PriceManager
└── loads 1986-2013.txt; lookup date / close / strike by index
```

### Qt GUI Modules

Accessible via the menu bar:
- Option pricing (`optionprice`)
- Implied volatility inversion (`implivola`)
- GARCH volatility estimation (`garchwidget`)
- Greeks and hedging parameters (`hedgepara`)
- Arbitrage portfolios and P&L curves (`arbitresult` + `curveplotwidget`)

## 6. Key Findings

- **Butterfly / Hawk**: profitable in low-volatility regimes; the Hawk is more tolerant of small fluctuations but yields lower per-trade returns.
- **Straddle / Strangle**: with relatively stable Hang Seng volatility over the sample period, straddle returns are unimpressive; the strangle is preferable thanks to its lower entry cost.
- **Volatility estimation**: GARCH(1,1)-implied volatility aligns reasonably well with long-run market levels, making it acceptable as a pricing input.
- **Implied volatility**: Newton's method converges quickly given a good initial guess; bisection is slower but more stable. The two are used complementarily in practice.

## 7. Build and Run

### Core CLI Version

```bash
cd code/core
# Original project used Visual Studio with stdafx.h; remove that include if compiling with g++
g++ -std=c++11 BlackScholes.cpp PriceControl.cpp UnitTest.cpp -o bs
./bs
```

### Qt GUI Version

```bash
cd code/qt-app
qmake BlackScholes_01.pro
make
./BlackScholes_01
```

> Note: the original project targets Qt 4.x with MSVC. To migrate to Qt 5/6, replace `QtGui/QApplication` with `QtWidgets/QApplication` and remove deprecated APIs such as `QTextCodec::setCodecForTr`.

## 8. References

- Black, F. & Scholes, M. (1973). *The Pricing of Options and Corporate Liabilities*. Journal of Political Economy.
- Bollerslev, T. (1986). *Generalized autoregressive conditional heteroskedasticity*. Journal of Econometrics.
- Hull, J. C. *Options, Futures, and Other Derivatives* (covers BS derivation, Greeks, and arbitrage strategies).

---

*This directory mirrors the source code and methodology of a 2015 undergraduate thesis. The original docx thesis, supplementary experiment data, and proposal/mid-term reports are not uploaded (they contain personal information). Only the core source code and methodological summaries are synced here.*
