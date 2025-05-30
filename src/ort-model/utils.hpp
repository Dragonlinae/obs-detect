#ifndef _EdgeYOLO_CPP_UTILS_HPP
#define _EdgeYOLO_CPP_UTILS_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "types.hpp"

static std::vector<std::string> read_class_labels_file(file_name_t file_name)
{
	std::vector<std::string> class_names;
	std::ifstream ifs(file_name);
	std::string buff;
	if (ifs.fail()) {
		return class_names;
	}
	while (getline(ifs, buff)) {
		if (buff == "")
			continue;
		class_names.push_back(buff);
	}
	return class_names;
}

const float color_list[80][3] = {
	{0.000f, 0.447f, 0.741f}, {0.850f, 0.325f, 0.098f}, {0.929f, 0.694f, 0.125f},
	{0.494f, 0.184f, 0.556f}, {0.466f, 0.674f, 0.188f}, {0.301f, 0.745f, 0.933f},
	{0.635f, 0.078f, 0.184f}, {0.300f, 0.300f, 0.300f}, {0.600f, 0.600f, 0.600f},
	{1.000f, 0.000f, 0.000f}, {1.000f, 0.500f, 0.000f}, {0.749f, 0.749f, 0.000f},
	{0.000f, 1.000f, 0.000f}, {0.000f, 0.000f, 1.000f}, {0.667f, 0.000f, 1.000f},
	{0.333f, 0.333f, 0.000f}, {0.333f, 0.667f, 0.000f}, {0.333f, 1.000f, 0.000f},
	{0.667f, 0.333f, 0.000f}, {0.667f, 0.667f, 0.000f}, {0.667f, 1.000f, 0.000f},
	{1.000f, 0.333f, 0.000f}, {1.000f, 0.667f, 0.000f}, {1.000f, 1.000f, 0.000f},
	{0.000f, 0.333f, 0.500f}, {0.000f, 0.667f, 0.500f}, {0.000f, 1.000f, 0.500f},
	{0.333f, 0.000f, 0.500f}, {0.333f, 0.333f, 0.500f}, {0.333f, 0.667f, 0.500f},
	{0.333f, 1.000f, 0.500f}, {0.667f, 0.000f, 0.500f}, {0.667f, 0.333f, 0.500f},
	{0.667f, 0.667f, 0.500f}, {0.667f, 1.000f, 0.500f}, {1.000f, 0.000f, 0.500f},
	{1.000f, 0.333f, 0.500f}, {1.000f, 0.667f, 0.500f}, {1.000f, 1.000f, 0.500f},
	{0.000f, 0.333f, 1.000f}, {0.000f, 0.667f, 1.000f}, {0.000f, 1.000f, 1.000f},
	{0.333f, 0.000f, 1.000f}, {0.333f, 0.333f, 1.000f}, {0.333f, 0.667f, 1.000f},
	{0.333f, 1.000f, 1.000f}, {0.667f, 0.000f, 1.000f}, {0.667f, 0.333f, 1.000f},
	{0.667f, 0.667f, 1.000f}, {0.667f, 1.000f, 1.000f}, {1.000f, 0.000f, 1.000f},
	{1.000f, 0.333f, 1.000f}, {1.000f, 0.667f, 1.000f}, {0.333f, 0.000f, 0.000f},
	{0.500f, 0.000f, 0.000f}, {0.667f, 0.000f, 0.000f}, {0.833f, 0.000f, 0.000f},
	{1.000f, 0.000f, 0.000f}, {0.000f, 0.167f, 0.000f}, {0.000f, 0.333f, 0.000f},
	{0.000f, 0.500f, 0.000f}, {0.000f, 0.667f, 0.000f}, {0.000f, 0.833f, 0.000f},
	{0.000f, 1.000f, 0.000f}, {0.000f, 0.000f, 0.167f}, {0.000f, 0.000f, 0.333f},
	{0.000f, 0.000f, 0.500f}, {0.000f, 0.000f, 0.667f}, {0.000f, 0.000f, 0.833f},
	{0.000f, 0.000f, 1.000f}, {0.000f, 0.000f, 0.000f}, {0.143f, 0.143f, 0.143f},
	{0.286f, 0.286f, 0.286f}, {0.429f, 0.429f, 0.429f}, {0.571f, 0.571f, 0.571f},
	{0.714f, 0.714f, 0.714f}, {0.857f, 0.857f, 0.857f}, {0.000f, 0.447f, 0.741f},
	{0.314f, 0.717f, 0.741f}, {0.50f, 0.5f, 0.0f}};

static void draw_objects(cv::Mat bgr, const std::vector<Object> &objects,
			 const std::vector<std::string> &class_names)
{

	for (size_t i = 0; i < objects.size(); i++) {
		const Object &obj = objects[i];

		int color_index = obj.label % 80;
		cv::Scalar color = cv::Scalar(color_list[color_index][0],
					      color_list[color_index][1],
					      color_list[color_index][2]);
		float c_mean = (float)(cv::mean(color)[0]);
		cv::Scalar txt_color;
		if (c_mean > 0.5) {
			txt_color = cv::Scalar(0, 0, 0);
		} else {
			txt_color = cv::Scalar(255, 255, 255);
		}

		cv::rectangle(bgr, obj.rect, color * 255, 2);

		char text[256];
		snprintf(text, sizeof(text), "%s %.1f%%", class_names[obj.label].c_str(),
			 obj.prob * 100);

		int baseLine = 0;
		cv::Size label_size =
			cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, &baseLine);

		cv::Scalar txt_bk_color = color * 0.7 * 255;

		int x = (int)(obj.rect.x);
		int y = (int)(obj.rect.y + 1);
		if (y > bgr.rows)
			y = bgr.rows;

		cv::rectangle(bgr,
			      cv::Rect(cv::Point(x, y),
				       cv::Size(label_size.width, label_size.height + baseLine)),
			      txt_bk_color, -1);

		cv::putText(bgr, text, cv::Point(x, y + label_size.height),
			    cv::FONT_HERSHEY_SIMPLEX, 0.4, txt_color, 1);

		// write the id of the object
		snprintf(text, sizeof(text), "ID: %d", (int)obj.id);
		cv::putText(bgr, text, cv::Point(x, y + label_size.height + 15),
			    cv::FONT_HERSHEY_SIMPLEX, 0.4, txt_color, 1);
	}
}

#endif
