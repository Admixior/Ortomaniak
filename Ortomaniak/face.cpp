#include "face.h"


Face::Face(void):specjal(false),heart_show(false)
{
	bad_odp_img.LoadFromMemory(zaladuj(IDB_FUU,L"PNG"),rozmiar(IDB_FUU,L"PNG"));
	good_odp_img.LoadFromMemory(zaladuj(IDB_JEA,L"PNG"),rozmiar(IDB_JEA,L"PNG"));

	good_scores_img.LoadFromMemory(zaladuj(IDB_BICZ,L"PNG"),rozmiar(IDB_BICZ,L"PNG"));
	bad_scores_img.LoadFromMemory(zaladuj(IDB_MATKO,L"PNG"),rozmiar(IDB_MATKO,L"PNG"));

	serce.LoadFromMemory(zaladuj(IDB_HEART,L"PNG"),rozmiar(IDB_HEART,L"PNG"));

	lewe_serce.SetImage(serce);
	prawe_serce.SetImage(serce);
	face_draw.SetImage(good_odp_img);

	lewe_serce.SetPosition(START_HEART_LEFT);
	prawe_serce.SetPosition(START_HEART_RIGHT);
	lewe_serce.SetCenter(serce.GetWidth()/2.f, serce.GetHeight()/2.f);
	prawe_serce.SetCenter(serce.GetWidth()/2.f, serce.GetHeight()/2.f);
	face_draw.SetPosition(START_FACE);
}


Face::~Face(void)
{
}

int Face::RescaleHeart()
{
	if(!heart_show)return -1;
	float scale=timer.GetElapsedTime()*2.f;
	if(scale>1.f)scale=2.f-scale;
	lewe_serce.SetScale(scale,scale);
	prawe_serce.SetScale(scale,scale);
	if(timer.GetElapsedTime()>1.f)return -1;
	return 0;
}

int Face::god_odp()
{
	face_draw.SetImage(good_odp_img);
	specjal=true;
	heart_show=true;
	timer.Reset();
	return 0;
}
int Face::bad_odp()
{
	specjal=true;
	face_draw.SetImage(bad_odp_img);
	heart_show=false;
	timer.Reset();
	return 0;
}
int Face::good_scores()
{
	face_draw.SetImage(good_scores_img);
	heart_show=false;
	specjal=false;
	return 0;
}
int Face::bad_scores()
{
	face_draw.SetImage(bad_scores_img);
	heart_show=false;
	specjal=false;
	return 0;
}
int Face::reset()
{
	face_draw.SetImage(good_odp_img);
	specjal=false;
	heart_show=false;
	return 0;
}
int Face::Draw(RenderWindow& wnd)
{
	wnd.Draw(face_draw);
	if(specjal)
	{
		if(timer.GetElapsedTime()>1.f)
			reset();
		if(!RescaleHeart())
		{
			wnd.Draw(lewe_serce);
			wnd.Draw(prawe_serce);
		}
	}
	return specjal;
}