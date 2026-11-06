# PantryPal: Recipe Finder 🍳

PantryPal is a C++ command-line tool that allows users to search for recipes based on the ingredients they currently have in their kitchen. It compares the performance of a custom-built Hash Map vs. a Red-Black Tree.

## Group 45 Members
* Elijah Baez (`elijahbaez`)
* Santiago Gomez (`santigom`)
* Alexandria Conaway (`aconaway3`)

## Prerequisites
1. **C++14 or higher**
2. **CMake 3.10 or higher**
3. **Dataset:** Download the RAW_recipes.csv file from the Food.com Kaggle Dataset (https://www.kaggle.com/datasets/shuyangli94/food-com-recipes-and-user-interactions?select=RAW_recipes.csv).

## How to Build and Run

1. Clone this repository to your local machine.
2. Place the RAW_recipes.csv dataset directly into the cmake-build-debug folder (or the root directory where the executable is generated).
3. Build the project using CMake:
   ```bash
   cmake .
   make
   ```
4. Run the executable:
   ```bash
   ./PantryPal
   ```
