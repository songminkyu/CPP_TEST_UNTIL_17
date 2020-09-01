#include "stdafx.h"
#include <iostream>
#include <optional>

struct image { }; // �̹��� ����ü

std::optional<image> crop_to_fox(std::optional<image>&) { std::cout << "crop_to_fox" << std::endl; 	return image{}; }
std::optional<image> bow_tie(std::optional<image>&) { std::cout << "bow_tie" << std::endl; return {}; }			// ���쿡�� ��Ÿ�̸� �����ش�.
std::optional<image> eye_sparkle(std::optional<image>&) { std::cout << "eye_sparkle" << std::endl; return {}; }		// ������ ���� ������ ���ش�.
std::optional<image> rainbow(std::optional<image>&) { std::cout << "rainbow" << std::endl; return {}; }			// ��濡 �������� �׷��ش�. 

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

std::optional<image> get_fox_image(std::optional<image>& img) {				// �̹����� ���찡 ���ԵǾ��ִٸ� ���� �̹����� �ּ���.
	auto compos_fn = Composer<crop_to_fox, bow_tie, eye_sparkle, rainbow >{};
	return compos_fn(img);
}

int main() {
	std::optional<image> img;
	get_fox_image(img);
	return 0;
}
