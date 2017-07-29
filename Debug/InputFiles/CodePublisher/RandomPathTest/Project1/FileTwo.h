#pragma once
#include <iostream>

namespace datastructures {

	namespace core {
		struct Stock {
			char * stockname;
			double stockprice;
			double currentchange;
		};

		class Window {
			int width;
			int height;
			int x;
			int y;

			char * glassType;

			inline Window createWindow() {
				Window w;
				return w;
			}
		};
	}
}