#include "big_int.hpp"

#include <limits>

template<typename T>
big_int<T> big_int<T>::get_shortest(const big_int<T> &first, const big_int<T> &second) {
    return first.pieces.size() > second.pieces.size() ? second : first;
}

template<typename T>
big_int<T> big_int<T>::get_longest(const big_int<T> &first, const big_int<T> &second) {
    return first.pieces.size() > second.pieces.size() ? first : second;
}

template<typename T>
std::string big_int<T>::binary_str() const {
    std::string output;

    T mask = 0b1;

    // Careful! This loop intentionally iterates from pieces.size() till 1, because loop from pieces.size() - 1 till 0
    //     will cause unsigned overflow, what will lead to an infinite loop.
    for (size_type i = pieces.size(); i > 0; --i) {
        T piece = pieces[i - 1];
        std::string number_str;
        for (size_type j = 0; j < std::numeric_limits<T>::digits; ++j) {
            number_str += ('0' + (piece & mask));
            piece >>= 1;
        }
        output += std::string(number_str.rbegin(), number_str.rend());
//        output += "|";
    }

    return output;
}

template<typename T>
big_int<T>::big_int(std::vector<T> initial_pieces, uint8_t initial_sign):
        pieces(std::move(initial_pieces)),
        sign(initial_sign) {
    if (initial_sign != 0 && initial_sign != 1) {
        throw std::invalid_argument("Sign should be 0 for positive numbers or 1 for negative");
    }
}

template<typename T>
big_int<T> big_int<T>::operator+(const big_int<T> &summand) const {
    T carry = 0;
    big_int<T> out({}, summand.sign);

    const big_int<T> &longest_summand = big_int<T>::get_longest(*this, summand);
    const big_int<T> &shortest_summand = big_int<T>::get_shortest(*this, summand);

    size_type max_size = longest_summand.pieces.size();
    size_type min_size = shortest_summand.pieces.size();
    out.pieces.resize(max_size);

    T fill_value = shortest_summand.sign ? std::numeric_limits<T>::max() : 0b0;

    for (size_type i = 0; i < max_size; ++i) {
        T value;

        if (i >= min_size) {
            value = longest_summand.pieces[i] + fill_value + carry;
        } else {
            value = longest_summand.pieces[i] + shortest_summand.pieces[i] + carry;
        }

        carry = longest_summand.pieces[i] > value;

        out.pieces[i] = value;
    }

    if (carry == 1) {
        out.pieces.push_back(carry);
    }

    return out;
}

template<typename T>
big_int<T> big_int<T>::operator~() const {
    big_int<T> out({}, !sign);
    out.pieces.resize(pieces.size());

    for(size_type i = 0; i < pieces.size(); ++i) {
        out.pieces[i] = ~pieces[i];
    }

    return out;
}

template<typename T>
big_int<T> big_int<T>::operator-() const {
    const big_int<T> &self = *this;
    const big_int<T> &one = big_int<T>({1}, 0);
    return ~self + one;
}

template<typename T>
big_int<T> big_int<T>::operator<<(const size_type &shift_by) const {
    big_int<T> output({}, sign);

    output.pieces.resize(pieces.size());

    size_type piece_shift = shift_by % std::numeric_limits<T>::digits;
    size_type piece_shift_complement = std::numeric_limits<T>::digits - piece_shift;

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

    T fill_value = output.sign ? std::numeric_limits<T>::max() : 0b0;

    if (additional_piece != fill_value) {
        output.pieces.push_back(additional_piece);
    }

    size_type empty_piece_count = shift_by / std::numeric_limits<T>::digits;
    if (empty_piece_count > 0) {
        output.pieces.insert(output.pieces.begin(), empty_piece_count, fill_value);
    }

    return output;
}

template<typename T>
big_int<T> big_int<T>::operator*(const big_int<T> &multiplicand) const {
    const big_int<T> &longest_multiplicand = big_int<T>::get_longest(*this, multiplicand);
    const big_int<T> &shortest_multiplicand = big_int<T>::get_shortest(*this, multiplicand);

    size_type min_size = shortest_multiplicand.pieces.size();
    size_type shift_size = std::numeric_limits<T>::digits;

    big_int<T> output({}, sign ^ multiplicand.sign);

    T mask = 0b1;

    for (size_type i = 0; i < min_size; ++i) {
        for (size_type j = 0; j < shift_size; ++j) {
            T current_mask = mask << j;
            T current_piece = shortest_multiplicand.pieces[i];

            if (current_piece & current_mask) {
                output = output + (longest_multiplicand << (size_type) (j + i * shift_size));
            }
        }
    }

    return output;
}
