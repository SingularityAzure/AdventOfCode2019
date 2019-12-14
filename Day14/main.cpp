#include <AzCore/IO/LogStream.hpp>
#include <AzCore/memory.hpp>
#include <AzCore/Time.hpp>
#include <AzCore/math.hpp>

using namespace AzCore;

io::LogStream cout("day14.log");

Array<char> input;

struct Ingredient;

struct RecipePart {
    i32 ingredientIndex;
    i64 quantity = 0;
    i64 FindOreCost(i64 count, Array<Ingredient> &ingredients);
};

struct Recipe {
    Array<RecipePart> parts{};
    i64 yield = 0;
    i64 FindOreCost(i64 count, i32 indexForUnused, Array<Ingredient> &ingredients);
};

struct Ingredient {
    Array<Recipe> recipes{};
    i64 reserves = 0; // Carry waste from different recursions
    i32 index;
    i64 FindOreCost(i64 count, Array<Ingredient> &ingredients) {
        if (recipes.size == 0) {
            // we are ore
            return count;
        }
        i64 result = INT64_MAX;
        Array<Ingredient> winningIngredients;
        for (Recipe &recipe : recipes) {
            // To permute we need a copy that's independent from other branches
            Array<Ingredient> thisRecipeIngredients(ingredients);
            i64 cost = recipe.FindOreCost(count, index, thisRecipeIngredients);
            if (cost < result) {
                result = cost;
                winningIngredients = thisRecipeIngredients;
            }
        }
        ingredients = winningIngredients;
        return result;
    }
};

i64 RecipePart::FindOreCost(i64 count, Array<Ingredient> &ingredients) {
    Ingredient &ingredient = ingredients[ingredientIndex];
    count *= quantity;
    i64 reservesTaken = min(ingredient.reserves, count);
    ingredient.reserves -= reservesTaken;
    count -= reservesTaken;
    if (count == 0) {
        return 0;
    }
    return ingredient.FindOreCost(count, ingredients);
}

i64 Recipe::FindOreCost(i64 count, i32 indexForUnused, Array<Ingredient> &ingredients) {
    // Round up divide
    i32 mod = count % yield;
    bool hasUnused = mod != 0;
    count = count / yield + i64(hasUnused);
    i64 cost = 0;
    for (RecipePart part : parts) {
        cost += part.FindOreCost(count, ingredients);
    }
    if (hasUnused) {
        ingredients[indexForUnused].reserves += yield - mod;
    }
    return cost;
}

Array<Ingredient> allIngredients;

Map<String, Ptr<Ingredient>> ingredients;

int main() {
    cout << "Day 14:" << std::endl;
    ClockTime start = Clock::now();

    input = FileContents("Day14/input.txt");
    // input = FileContents("Day14/input.txt");
    // remove commas
    for (i32 i = input.size-1; i >= 0; i--) {
        if (input[i] == ',') {
            input.Erase(i);
            i++;
        }
    }
    Array<Range<char>> words = SeparateByValues(&input, {' ', '\n'});

    { // Getting our recipes from the data
        Array<RecipePart> recipeParts;
        bool nextIsRecipeTarget = false;
        for (i32 i = 0; i < words.size; i++) {
            Range<char> &word1 = words[i];
            if (String(word1) == "=>") {
                nextIsRecipeTarget = true;
                continue;
            }
            i64 count = StringToI64(word1);
            Range<char> &word2 = words[++i];
            // cout << "Word 1 is " << String(word1) << ", word 2 is " << String(word2) << std::endl;
            if (ingredients.count(word2) == 0) {
                allIngredients.Append(Ingredient());
                allIngredients.Back().index = allIngredients.size-1;
                ingredients[word2] = allIngredients.GetPtr(allIngredients.size-1);
            }
            Ptr<Ingredient> ingredient = ingredients[word2];
            if (nextIsRecipeTarget) {
                // This marks the completion of a recipe
                Recipe recipe;
                recipe.parts = std::move(recipeParts); // This empties recipeParts
                recipe.yield = count;
                ingredient->recipes.Append(recipe);
                nextIsRecipeTarget = false;
            } else {
                // Just one component of a recipe
                RecipePart part;
                part.ingredientIndex = ingredient.index;
                part.quantity = count;
                recipeParts.Append(part);
            }
        }
    }
    for (auto &pair : ingredients) {
        Array<Ingredient> ing(allIngredients);
        i64 oreCost = pair.second->FindOreCost(1, ing);
        cout << "Ingredient " << pair.first << " has an ore cost of " << oreCost << " for 1." << std::endl;
    }
    Ptr<Ingredient> fuel = ingredients["FUEL"];
    Array<Ingredient> ing(allIngredients);
    i64 oreCostForOneFuel = fuel->FindOreCost(1, ing);
    cout << "Part 1: Ore cost for 1 fuel is " << oreCostForOneFuel << " ore." << std::endl;

    ing = allIngredients;
    i64 oreCostForOneThousandFuel = fuel->FindOreCost(1000, ing);

    i64 maxFuel;
    i64 moveBy = 100;
    for (maxFuel = 1000000000000000/oreCostForOneThousandFuel;; maxFuel += moveBy) {
        ing = allIngredients;
        i64 oreCost = fuel->FindOreCost(maxFuel, ing);
        if (oreCost > 1000000000000) {
            if (moveBy > 0) {
                moveBy = -moveBy / 2;
            }
        } else {
            if (moveBy < 0) {
                moveBy = -moveBy / 2;
            }
        }
        if (moveBy == 0) break;
    }
    cout << "Part 2: Fuel that can be created from a trillion ore: " << maxFuel << std::endl;

    cout << "Total time taken: " << FormatTime(Clock::now() - start) << std::endl;

    return 0;
}
