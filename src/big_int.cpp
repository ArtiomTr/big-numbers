#include "big_int.hpp"

#include <limits>
#include <regex>

template<typename T>
big_int<T> big_int<T>::get_shortest(const big_int<T> &first, const big_int<T> &second) {
    return first.pieces.size() > second.pieces.size() ? second : first;
}

template<typename T>
big_int<T> big_int<T>::get_longest(const big_int<T> &first, const big_int<T> &second) {
    return first.pieces.size() > second.pieces.size() ? first : second;
}

template<typename T>
T big_int<T>::get_fill_value() const {
    return sign ? std::numeric_limits<T>::max() : 0b0;
}

template<typename T>
std::size_t big_int<T>::get_box_size() {
    return sizeof(T) * std::numeric_limits<uint8_t>::digits;
}

template<typename T>
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
    big_int<T> out({}, 0);

    const big_int<T> &longest_summand = big_int<T>::get_longest(*this, summand);
    const big_int<T> &shortest_summand = big_int<T>::get_shortest(*this, summand);

    size_type max_size = longest_summand.pieces.size();
    size_type min_size = shortest_summand.pieces.size();
    out.pieces.resize(max_size);

    T carry = 0;

    for (size_type i = 0; i < max_size; ++i) {
        T value;

        if (i >= min_size) {
            value = longest_summand.pieces[i] + shortest_summand.get_fill_value() + carry;
        } else {
            value = longest_summand.pieces[i] + shortest_summand.pieces[i] + carry;
        }

        carry = longest_summand.pieces[i] > value;

        out.pieces[i] = value;
    }

    T additionalPiece = longest_summand.get_fill_value() + shortest_summand.get_fill_value();

    if(carry == 1) {
        additionalPiece += carry;
    }

    out.sign = additionalPiece >> (big_int<T>::get_box_size() - 1);

    if(additionalPiece != out.get_fill_value()) {
        out.pieces.push_back(additionalPiece);
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
    big_int<T> output = ~(*this) + big_int<T>({1}, 0);
    output.sign = !sign;
    return output;
}

template<typename T>
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

template<typename T>
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
                output = output + (longest_multiplicand << (size_type) (j + i * shift_size));
            }
        }
    }

    return output;
}

std::vector<uint8_t> source_to_binary(const std::string &source) {
    std::vector<uint8_t> binary_numbers;

    std::vector<uint8_t> transformed_source;
    transformed_source.resize(source.size());

    std::transform(source.begin(), source.end(), transformed_source.begin(), [](char in) {
        return in - '0';
    });

    while(!transformed_source.empty()) {
        bool carry = false;
        uint8_t &current = transformed_source.back();

        if(current % 2 == 0) {
            binary_numbers.push_back(0);
        } else {
            binary_numbers.push_back(1);
            current -= 1;
        }

        for(uint8_t &member : transformed_source) {
            bool oldCarry = carry;
            carry = member % 2;
            member /= 2;
            if(oldCarry) {
                member += 5;
            }
        }

        if(transformed_source[0] == 0) {
            transformed_source.erase(transformed_source.begin());
        }
    }

    return binary_numbers;
}

template<typename T>
big_int<T> parse_big_int(std::string source) {
    std::regex big_int_regex("^-?\\d+$");

    if(!std::regex_match(source, big_int_regex)) {
        throw std::invalid_argument("Invalid big_int format");
    }

    typedef typename std::vector<T>::size_type size_type;

    uint8_t sign = source[0] == '-';
    std::cout << source[0] << " " << (int) sign << std::endl;

    if(sign) {
        source.erase(source.begin());
    }

    std::vector<uint8_t> binary = source_to_binary(source);

    std::vector<T> pieces;
    size_type box_count = binary.size() / big_int<T>::get_box_size();

    pieces.resize(box_count);

    for(size_type i = 0; i < box_count; ++i) {
        T box_value = 0;
        for(size_type j = 0; j < big_int<T>::get_box_size(); ++j) {
            uint8_t bit = *(binary.begin() + i * big_int<T>::get_box_size() + j);
            box_value |= (bit << j);
        }
        pieces[i] = box_value;
    }

    size_type left = binary.size() % big_int<T>::get_box_size();

    if(box_count * big_int<T>::get_box_size() < binary.size()) {
        T box_value = 0;
        for(size_type i = 0; i < left; ++i) {
            uint8_t bit = *(binary.begin() + box_count * big_int<T>::get_box_size() + i);
            box_value |= (bit << i);
        }
        pieces.push_back(box_value);
    }

    for(auto i = binary.rbegin(); i < binary.rend(); ++i) {
        std::cout << (int) *i;
    }
    std::cout << std::endl;

    if(sign) {
        return -big_int(pieces, 0);
    }

    return big_int<T>(pieces, sign);
}