#include "stdafx.h"
#include <iostream>
#include <optional>

struct image { }; // 이미지 구조체

std::optional<image> crop_to_fox(std::optional<image>&) { std::cout << "crop_to_fox" << std::endl; 	return image{}; }
std::optional<image> bow_tie(std::optional<image>&) { std::cout << "bow_tie" << std::endl; return {}; }			// 여우에게 넥타이를 묶어준다.
std::optional<image> eye_sparkle(std::optional<image>&) { std::cout << "eye_sparkle" << std::endl; return {}; }		// 여우의 눈을 빛나게 해준다.
std::optional<image> rainbow(std::optional<image>&) { std::cout << "rainbow" << std::endl; return {}; }			// 배경에 무지개를 그려준다. 

template<auto FN, auto... Others>
struct Composer;

template<auto FN>
struct Composer<FN> {
	template<typename Arg>
	auto operator()(Arg&& arg) {
		return FN(arg);
	}
};

template<auto FN, auto... Others>
struct Composer {
	template<typename Arg>
	auto operator()(Arg&& arg) {
		auto rt = FN(arg);
		if (rt) return  Composer<Others...>{}(rt);
		return decltype(rt){};
	}
};

std::optional<image> get_fox_image(std::optional<image>& img) {				// 이미지에 여우가 포함되어있다면 여우 이미지를 주세요.
	auto compos_fn = Composer<crop_to_fox, bow_tie, eye_sparkle, rainbow >{};
	return compos_fn(img);
}

int main() {
	std::optional<image> img;
	get_fox_image(img);
	return 0;
}
