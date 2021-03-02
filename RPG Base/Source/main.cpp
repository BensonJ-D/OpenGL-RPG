#include "interface.h"

int main(int argc, char* argv[])
{
	Game.Setup(argc, argv);

	return 0;
}

void GameObject::Main()
{	
	Game.Redraw();
}

void GameObject::Update(int i)
{
	for (int button = 0; button < 3; button++)
	if (KeyStates.mouseState[button] == 1)
		KeyStates.mouseState[button] = 2;

	for (auto const &iter : objects)
	for (auto const &iter2 : iter)
	{
		if (iter2.second == nullptr)
		{
			objects[i].erase(iter2.first);
			continue;
		}
		iter2.second->Update();
	}

	glutTimerFunc(1000/30, Game.Update, 0);
}

void GameObject::Redraw()
{
	glColor3ub(255, 255, 255);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	for (int i = 0; i < MAX_LAYERS; i++)
	{
		level.Draw(i);

		for (auto const &iter2 : objects[i])
		{
			if (iter2.second == nullptr)
				continue;
			
			iter2.second->Draw();
		}
	}
	
	glutSwapBuffers();
}