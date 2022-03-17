#include "big_int.hpp"

#include <limits>
#include <regex>

#include "parsing_utils.h"

template<class T>
big_int<T> big_int<T>::get_shortest(const big_int<T> &first, const big_int<T> &second) {
    return first.pieces.size() > second.pieces.size() ? second : first;
}

template<class T>
big_int<T> big_int<T>::get_longest(const big_int<T> &first, const big_int<T> &second) {
    return first.pieces.size() > second.pieces.size() ? first : second;
}

template<class T>
T big_int<T>::get_fill_value() const {
    return sign ? std::numeric_limits<T>::max() : 0b0;
}

template<class T>
std::size_t big_int<T>::get_box_size() {
    return sizeof(T) * std::numeric_limits<uint8_t>::digits;
}

template<class T>
std::string big_int<T>::binary_str() const {
    std::string output = sign ? "-" : "+";

    T mask = 0b1;

    // Careful! This loop intentionally iterates from pieces.size() till 1, because loop from pieces.size() - 1 till 0
    //     will cause unsigned overflow, what will lead to an infinite loop.
    for (size_type i = pieces.size(); i > 0; --i) {
        T piece = pieces[i - 1];
        std::string number_str;
        for (size_type j = 0; j < big_int<T>::get_box_size(); ++j) {
            number_str += ('0' + (piece & mask));
            piece >>= 1;
        }
        output += std::string(number_str.rbegin(), number_str.rend());
    }

    return output;
}

template<class T>
big_int<T>::big_int(std::vector<T> initial_pieces, uint8_t initial_sign, pad<T> *initial_value_pad):
        pieces(std::move(initial_pieces)),
        sign(initial_sign),
        value_pad(initial_value_pad) {
    if (initial_sign != 0 && initial_sign != 1) {
        throw std::invalid_argument("Sign should be 0 for positive numbers or 1 for negative");
    }
}

template<class T>
big_int<T>::big_int(std::vector<T> initial_pieces, uint8_t initial_sign):
        big_int(initial_pieces, initial_sign, new default_pad<T>()) {
}

template<class T>
big_int<T> big_int<T>::operator+(const big_int<T> &summand) const {
    big_int<T> out({}, 0);

    const big_int<T> &longest_summand = big_int<T>::get_longest(*this, summand);

    size_type max_size = longest_summand.pieces.size();
    out.pieces.resize(max_size);

    padded_list<T> *first_summand_pieces = value_pad->pad_for_sum(summand.pieces, summand.sign);
    padded_list<T> *second_summand_pieces = value_pad->pad_for_sum(pieces, sign);

    T carry = 0;

    for (size_type i = 0; i < max_size; ++i) {
        T value = first_summand_pieces->get(i) + second_summand_pieces->get(i) + carry;

        carry = (first_summand_pieces->get(i) > value) || (second_summand_pieces->get(i) > value) ||
                (carry && (first_summand_pieces->get(i) == value || second_summand_pieces->get(i) == value));

        out.pieces[i] = value;
    }

    T additional = first_summand_pieces->get(max_size) + second_summand_pieces->get(max_size) + carry;

    out.sign = additional >> (big_int<T>::get_box_size() - 1);

    if (additional != out.get_fill_value()) {
        out.pieces.push_back(additional);
    }

    delete first_summand_pieces;
    delete second_summand_pieces;

    return out;
}

template<class T>
big_int<T> big_int<T>::operator-(const big_int<T> &subtrahend) const {
    const big_int<T> &minuend = *this;

    return minuend + (-subtrahend);
}

template<class T>
big_int<T> big_int<T>::operator~() const {
    big_int<T> out({}, !sign);
    out.pieces.resize(pieces.size());

    for (size_type i = 0; i < pieces.size(); ++i) {
        out.pieces[i] = ~pieces[i];
    }

    return out;
}

template<class T>
big_int<T> big_int<T>::operator-() const {
    big_int<T> output = ~(*this) + big_int<T>({1}, 0);
    output.sign = !sign;
    return output;
}

template<class T>
big_int<T> big_int<T>::operator<<(const size_type &shift_by) const {
    big_int<T> output({}, sign);

    output.pieces.resize(pieces.size());

    size_type piece_shift = shift_by % big_int<T>::get_box_size();
    size_type piece_shift_complement = big_int<T>::get_box_size() - piece_shift;

    T maskBuilder = 0b0;

    for (size_type i = 0; i < piece_shift_complement; ++i) {
        maskBuilder <<= 1;
        maskBuilder += 1;
    }

    if (maskBuilder == 0) {
        maskBuilder = std::numeric_limits<T>::max();
    }

    T mask = ~maskBuilder;

    if (!pieces.empty()) {
        output.pieces[0] = (pieces[0] << piece_shift);
    }

    for (size_type i = 1; i < pieces.size(); ++i) {
        output.pieces[i] = (pieces[i] << piece_shift) |
                           ((pieces[i - 1] & mask) >> piece_shift_complement);
    }

    T additional_piece = ((pieces[output.pieces.size() - 1] & mask) >> piece_shift_complement);

    T fill_value = output.get_fill_value();

    if (additional_piece != fill_value) {
        output.pieces.push_back(additional_piece);
    }

    size_type empty_piece_count = shift_by / big_int<T>::get_box_size();
    if (empty_piece_count > 0) {
        output.pieces.insert(output.pieces.begin(), empty_piece_count, fill_value);
    }

    return output;
}

template<class T>
big_int<T> big_int<T>::operator*(const big_int<T> &multiplicand) const {
    const big_int<T> &longest_multiplicand = big_int<T>::get_longest(*this, multiplicand);
    const big_int<T> &shortest_multiplicand = big_int<T>::get_shortest(*this, multiplicand);

    size_type min_size = shortest_multiplicand.pieces.size();
    size_type shift_size = big_int<T>::get_box_size();

    big_int<T> output({}, sign ^ multiplicand.sign);

    T mask = 0b1;

    for (size_type i = 0; i < min_size; ++i) {
        for (size_type j = 0; j < shift_size; ++j) {
            T current_mask = mask << j;
            T current_piece = shortest_multiplicand.pieces[i];

            if (current_piece & current_mask) {
                big_int<T> summand = (longest_multiplicand << (size_type) (j + i * shift_size));
                output = output + summand;
            }
        }
    }

    return output;
}

template<class T>
std::vector<T> split_into_boxes(const std::vector<uint8_t> &bits) {
    typedef typename std::vector<T>::size_type size_type;

    std::vector<T> pieces;
    size_type box_count = bits.size() / big_int<T>::get_box_size();

    pieces.resize(box_count);

    for (size_type i = 0; i < box_count; ++i) {
        T box_value = 0;
        for (size_type j = 0; j < big_int<T>::get_box_size(); ++j) {
            uint8_t bit = *(bits.rbegin() + i * big_int<T>::get_box_size() + j);
            box_value |= (bit << j);
        }
        pieces[i] = box_value;
    }

    size_type left = bits.size() % big_int<T>::get_box_size();

    if (box_count * big_int<T>::get_box_size() < bits.size()) {
        T box_value = 0;
        for (size_type i = 0; i < left; ++i) {
            uint8_t bit = *(bits.rbegin() + box_count * big_int<T>::get_box_size() + i);
            box_value |= (bit << i);
        }
        pieces.push_back(box_value);
    }

    return pieces;
}

template<class T>
std::strong_ordering big_int<T>::operator<=>(const big_int<T> &second_operand) const {
    const big_int<T> &first_operand = *this;

    if (first_operand.sign != second_operand.sign) {
        return second_operand.sign <=> first_operand.sign;
    }

    padded_list<T> *first_operand_pieces = value_pad->pad_for_sum(first_operand.pieces, first_operand.sign);
    padded_list<T> *second_operand_pieces = value_pad->pad_for_sum(second_operand.pieces, second_operand.sign);

    size_type longest = std::max(first_operand.pieces.size(), second_operand.pieces.size());

    for (size_type i = longest; i > 0; --i) {
        auto result = first_operand_pieces->get(i - 1) <=> second_operand_pieces->get(i - 1);

        if (result != std::strong_ordering::equal) {
            return result;
        }
    }

    return std::strong_ordering::equal;
}

template<typename T>
bool big_int<T>::operator==(const big_int<T> &second_operand) const {
    return (*this <=> second_operand) == std::strong_ordering::equal;
}

template<class T>
big_int<T> parse_big_int(std::string source) {
    std::regex big_int_regex("^-?\\d+$");

    if (!std::regex_match(source, big_int_regex)) {
        throw std::invalid_argument("Invalid big_int format");
    }

    uint8_t sign = source[0] == '-';

    if (sign) {
        source.erase(source.begin());
    }

    std::vector<uint8_t> binary = integral_source_to_binary(source);

    std::vector<T> pieces = split_into_boxes<T>(binary);

    big_int<T> out(pieces, 0);

    return sign ? -out : out;
}

template
class big_int<uint8_t>;

template big_int<uint8_t> parse_big_int(std::string source);