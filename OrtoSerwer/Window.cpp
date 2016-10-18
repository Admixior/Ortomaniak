#include "Window.h"


Window::Window(int _id_window)
	:id_window(_id_window)
{
	window = sfg::Window::Create();
}


Window::~Window(void)
{
}

void Window::EraseWindow()
{
	EventMem* em= new EventMem;
	em->type= EVENT_DESTROY_WINDOW;
	em->data= new int(id_window);
	event.push(em);
}

