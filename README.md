# KASUMI TMTO Attack

In this project, we analyse the real-world security of KASUMI as used in 2G and 3G networks in form of A5/3 towards time-memory trade-off attacks. KASUMI is a block cipher algorithm used in mobile embedded systems to provide security between the phone and the base station. In the security analysis, the performance optimization of the attack algorithm will play a major role.


## Kasumi cipher

The project includes an optimized version of the KASUMI cipher.

At the current state of the implemented cipher, the current benchmark results has been measured:


|                 overview        | Zenbook                | Yoga pro 2   
|:--------------------------------|:-----------------------|:-----------------------|
| CPU Frequency                   | 1,8 GHZ @ 2.9ghz TURBO | 2,0 GHZ @ 3.0ghz TURBO |
| CPU Cycles/s                    | 2900000000             | 3000000000             |
| Times encryption of 64 bit data | 10000000               | 10000000               |
| Total bit encyrpted             | 640000000              | 640000000              |


| Yoga 2 pro 2.0 ghz - cki impl. |||||
|:-------------------------------|:-|:-----|:-----|:----------|
|GCC compile flags.|Time in sec (average)|Cycles in tot|Cycles per bit|Cycles per byte|
| None             | 7,2322              | 20973380000 | 32,77090625  | 262,16725     |
| O2               | 2,0035              | 5810150000  | 9,078359375  | 72,626875     |
| O3               | 1,8945              | 5494050000  | 8,584453125  | 68,675625     |
| Ofast            | 1,897               | 5501300000  | 8,59578125   | 68,76625      |

| Yoga 2 pro 2.0 ghz - cki impl. |||||
|:-------------------------------|:-|:-----|:-----|:----------|
|GCC compile flags.|Time in sec (average)|Cycles in tot|Cycles per bit|Cycles per byte|
| None             | 6,885               | 20655000000 | 32,2734375   | 258,1875      |
| O2               | 1,933636364         | 5800909091  | 9,063920455  | 72,51136364   |
| O3               | 1,823333333         | 5470000000  | 8,546875     | 68,375        |
| Ofast            | 1,835               | 5505000000  | 8,6015625    | 68,8125       |


## Choice of parameters and table

The project will include scripts for calculating paramater choices for the TMTO attack. All scripts are made for sage mathmatics. 
