#include <bitset>
#include <iostream>
#include <vector>

/** Объект для побитовой записи (только для записи). */
class BitWriter {
public:
    BitWriter(): data_(), curr_bit_(0) {}
    /** Правило трех не забываем. */

    /** Пишем один бит в конец. */
    void
    WriteBit(bool bit)
    {
        /* Если мы не начали новый бит - создаем его. */
        if (curr_bit_ % BYTE_WIDTH == 0)
            data_.push_back(0);
        /*
         * Bits from left to right within a byte.
         * 00000000 - byte (8 bit)
         * ------->
         * 1 << 7 = 10000000
         * 1 << 6 = 01000000
         * ...
         */
        if (bit)
            data_.back() |= 1 << (7 - curr_bit_ % BYTE_WIDTH);
        curr_bit_++;
    }

    /** Пишем один байт (целиком) в конец. */
    void
    WriteByte(uint8_t byte)
    {
        /*
         * Если мы yt выровнены по границе байта, то приходится играть с масками:
         *
         *    11011  101 - Наш байт
         * 00000000 [00000000] - Данные
         *    ^
         *   curr_bit_
         * (Подробнее коммент в коде).
         *
         * Но если curr_bit_ % BYTE_WIDTH == 0, то мы уже выровнены:
         *                    11011101 - Наш байт
         * 00000000 00000000 [00000000]  - Данные
         *                    ^
         *                   curr_bit
         */
        if (curr_bit_ % BYTE_WIDTH == 0) {
            data_.push_back(byte);
        } else {
            /*
             * offset - позиция курсора в текущем байте
             *    11011  101 - Наш байт
             * 00000000 [00000000] - Данные
             *    ^
             *   curr_bit_
             *
             * Нужно взять правые (BYTE_WIDTH - offset) бит текущей ячейки
             * и установить туда левые (BYTE_WIDTH - offset) бит переданного байта.
             *
             * Остаток (offset правых битов переданного байта) идет в самые левые
             * биты следующего байта.
             */
            int offset = curr_bit_ % BYTE_WIDTH;
            data_.back() |= byte >> offset;
            data_.push_back(byte << (BYTE_WIDTH - offset));
        }
        curr_bit_ += BYTE_WIDTH;
    }

    size_t
    GetBitCount() const
    {
        return curr_bit_;
    }

    void
    Print() const
    {
        /*
         * Итерация по контейнеру, слева направо, берем элементы по ссылке.
         * Используем вспомогательный класс для печати битов.
         */
        for (auto &b: data_)
            std::cout << std::bitset<BYTE_WIDTH>(b) << "|";
        std::cout << std::endl;
        std::cout << "Bits in last byte: " << curr_bit_ % BYTE_WIDTH << std::endl;
    }

private:
    static constexpr size_t BYTE_WIDTH = 8;
    /** Данные (набор байт); */
    std::vector<uint8_t> data_;
    /** Текущий бит (который будем писать). */
    size_t curr_bit_;
};

int
main()
{
    BitWriter writer;
    writer.WriteBit(1);
    writer.WriteBit(1);
    writer.WriteBit(0);
    writer.WriteBit(1);
    writer.WriteByte(255);
    writer.WriteByte(0);
    writer.WriteBit(1);
    writer.WriteBit(1);
    writer.WriteBit(0);
    writer.WriteBit(1);
    writer.WriteByte(128);
    writer.Print();
}
