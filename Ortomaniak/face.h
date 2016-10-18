#pragma once
#include "orto.h"
using namespace sf;
class Face
{
	Image good_scores_img,
		bad_odp_img,
		good_odp_img,
		bad_scores_img,
		serce;
	Sprite face_draw,
		lewe_serce,
		prawe_serce;

	Clock timer;

	int RescaleHeart();
	bool heart_show
		,specjal;
public:
	int god_odp();
	int bad_odp();
	int good_scores();
	int bad_scores();
	int reset();
	int Draw(RenderWindow& wnd);
	Face(void);
	~Face(void);
};

