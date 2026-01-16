# Mini-Quantitative-Finance-Project

A C++ implementation of a signal processing system designed to aggregate and analyze financial market data observations. This project focuses on object-oriented design to handle trade data and calculate risk-adjusted exposure.

## Key Components

### 1. Observation Class
Represents a single "snapshot" or trade in the market.
* **Attributes**: Stores Price, Volume, and a Timestamp.
* **Dollar Value**: Includes a method to calculate the total monetary value of the trade (`Price * Volume`).
* **Formatted Output**: Implements a custom `operator<<` for clean, precision-controlled logging of trade data.

### 2. SignalSeries Container
A collection of `Observation` objects managed via a `std::vector`.
* **Data Aggregation**: Provides methods to push new data into the series while maintaining memory safety.
* **Risk Management**: Features a `RiskLVL` enumeration (LOW, MEDIUM, HIGH) that applies different weights to the signal.
* **Weighted Signal**: Calculates the total value of all observations in the series, adjusted by a specific risk multiplier.

## How it Works

The system converts raw market data into `Observation` instances. These are then processed by the `SignalSeries` to produce a single, actionable numerical signal based on the user's risk appetite:

- **LOW Risk**: 35% exposure to total signal value.
- **MEDIUM Risk**: 67% exposure to total signal value.
- **HIGH Risk**: 100% exposure to total signal value.

## Usage Example

```cpp
Observation trade(96601.02, 1.23, 4500);
SignalSeries series;
series.in(trade);

// Calculate total signal impact at Medium Risk
double result = series.weighted_signal(RiskLVL::MEDIUM);
