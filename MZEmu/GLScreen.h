#pragma once
#pragma comment (lib, "opengl32.lib")
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLScreen : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLScreen(int width, int height, QWidget* parent = nullptr);
    ~GLScreen();

    QImage screenBuffer;

private:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int heigh) override;

    GLuint textureId = 0;
    GLfloat verticles[8] = {0};

};
