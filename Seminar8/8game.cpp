#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

/* Final state. */
static constexpr int FIELD_SIZE = 9;
using Field = std::array<uint8_t, FIELD_SIZE>;
constexpr Field finish = {1, 2, 3, 4, 5, 6, 7, 8, 0};

/* Current game state. */
class GameState
{
public:
    GameState(const Field &field): field(field)
    {
        for (int i = 0; i < FIELD_SIZE; i++) {
            if (field[i] == 0) {
                emptyPos = i;
                break;
            }
        }
    }

    /* Whether we finished the final state. */
    bool
    IsComplete() const
    {
        return field == finish;
    }

    /* Whether we can move empty cell. */

    bool
    CanMoveLeft() const
    {
        return emptyPos % 3 != 0;
    }

    bool
    CanMoveRight() const
    {
        return emptyPos % 3 != 2;
    }

    bool
    CanMoveUp() const
    {
        return emptyPos > 2;
    }

    bool
    CanMoveDown() const
    {
        return emptyPos < 6;
    }

    /* Moving methods. */

    GameState MoveLeft() const
    {
        assert(CanMoveLeft());

        GameState newState(*this);
        std::swap(newState.field[emptyPos - 1], newState.field[emptyPos]);
        newState.emptyPos--;
        return newState;
    }

    GameState MoveRight() const
    {
        assert(CanMoveRight());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 1]);
        newState.emptyPos++;
        return newState;
    }

    GameState MoveUp() const
    {
        assert(CanMoveUp());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 3]);
        newState.emptyPos -= 3;
        return newState;
    }

    GameState MoveDown() const
    {
        assert(CanMoveDown());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 3]);
        newState.emptyPos += 3;
        return newState;
    }

    bool operator==(const GameState &other) const
    {
        return field == other.field;
    }

    /* Решаема только если число инверсий четно. */
    bool
    IsSolvable() const
    {
        return getInvCount() % 2 == 0;
    }

private:

    size_t
    getInvCount() const
    {
        size_t inv_count = 0;
        for (int i = 0; i < FIELD_SIZE - 1; i++) {
            for (int j = i + 1; j < FIELD_SIZE; j++) {
                if (field[i] != 0 && field[j] != 0 && field[i] > field[j])
                    inv_count++;
            }
        }
        return inv_count;
    }

    Field field;
    /* Coordinate of zero. */
    int emptyPos;

    friend struct GameStateHasher;
    friend std::ostream &operator<<(std::ostream &out, const GameState &state);
};

struct GameStateHasher
{
public:
    /* TODO: написать нормальный хэш. */
    size_t operator()(const GameState &state) const
    {
        size_t hash = 0;
        for (uint8_t i : state.field) {
            hash *= 2;
            hash += i;
        }
        return hash;
    }
};

std::ostream &operator<<(std::ostream &out, const GameState &state)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            out << static_cast<int>(state.field[i * 3 + j]) << ' ';
        }
        out << std::endl;
    }
    return out;
}

/**
 * Returns required steps in string format to solve the game.
 * Amount of steps is guaranteed to be the shortest.
 * Example: "UURDL".
 */
std::string
FindSteps(const Field &field)
{
    GameState startState(field);

    if (!startState.IsSolvable())
        return "<UNSOLVABLE>";

    /* Хранит факт посещения + шаг, которым посетили. */
    std::unordered_map<GameState, char, GameStateHasher> steps;
    steps[startState] = 'S';

    std::queue<GameState> queue;
    queue.push(startState);

    /* Step 1 - find the shortest path with BFS. */
    while (!queue.empty()) {
        GameState state = queue.front();
        queue.pop();

        if (state.IsComplete())
            break;

        if (state.CanMoveLeft()) {
            GameState newState = state.MoveLeft();
            if (steps.find(newState) == steps.end()) {
                steps[newState] = 'L';
                queue.push(newState);
            }
        }

        if (state.CanMoveRight()) {
            GameState newState = state.MoveRight();
            if (steps.find(newState) == steps.end()) {
                steps[newState] = 'R';
                queue.push(newState);
            }
        }

        if (state.CanMoveUp()) {
            GameState newState = state.MoveUp();
            if (steps.find(newState) == steps.end()) {
                steps[newState] = 'U';
                queue.push(newState);
            }
        }

        if (state.CanMoveDown()) {
            GameState newState = state.MoveDown();
            if (steps.find(newState) == steps.end()) {
                steps[newState] = 'D';
                queue.push(newState);
            }
        }
    }

    /* Step 2 - recover the shortest path. */
    std::string path;
    GameState state(finish);

    std::cout << state << std::endl;

    while (steps[state] != 'S') {
        char move = steps[state];
        path += move;
        switch (move) {
        case 'L': {
            state = state.MoveRight();
            break;
        }
        case 'R': {
            state = state.MoveLeft();
            break;
        }
        case 'D': {
            state = state.MoveUp();
            break;
        }
        case 'U': {
            state = state.MoveDown();
            break;
        }
        default:
            assert(false);
        }

        std::cout << state << std::endl;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

int
main(void)
{
    Field field = {1, 2, 3, 4, 5, 6, 0, 7, 8};
    std::cout << FindSteps(field) << std::endl;
    return 0;
}
