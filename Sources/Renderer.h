#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <QMutex>
#include <QTime>

#include <thread>
#include <atomic>

#include "Color.h"
#include "Ray.h"
#include "Intersection.h"

class Camera;
class Scene;
class KDTreeNode;
class Photon;
class PhotonMap;

class Renderer : public QWidget
{
    Q_OBJECT
public:
    Renderer(QWidget *parent = 0);
    virtual ~Renderer();

    void    setImageSize(int width, int height);
    QSize imageSize();

    Camera* camera() const;
    void    setCamera(Camera* camera);

    Scene*  scene() const;
    void    setScene(Scene* scene);

    void    render();
    void    play();
    void    pause();
    void    stopRendering();

    bool    isRendering() const;
    bool    isPaused() const;

    void    saveImage(QString const& filename);

    int     elapsedTime() const;
    int     sampleNumber() const;

signals:
    void    renderingStarted();
    void    renderingFinished();
    void    clicked(Intersection hit);

protected:
    void    paintEvent(QPaintEvent *e);
    void    mouseReleaseEvent(QMouseEvent *);

private slots:
    void    _onStop();

private:

    void    _raytrace();
    void    _raytraceSections();
    void    _throwRay(Ray const& ray, Intersection &hit);
    bool    _isShaded(QVector3D const& hitPosition, QVector3D const& toLight, QVector3D const& lightPos, float time);
    void    _setPixel(int x, int y, Color const& color);

    QImage  _image;

    Camera* _camera;
    Scene*  _scene;

    std::thread*        _raytracerThread;
    QList<std::thread*> _computingThreads;
    QList<QRect>        _renderingTasks;
    QMutex              _renderingTasksMutex;
    std::atomic<bool>   _stopThread;

    std::atomic<bool>   _isRendering;
    QMutex              _pauseMutex;
    std::atomic<bool>   _isPaused;

    QMutex              _renderingTimeMutex;
    QTime               _renderingTime;
    std::atomic<int>    _elapsedTime;

    std::atomic<int>    _sampleNumber;
    QVector<Color>      _imageColors;

    PhotonMap*          _globalPhotonMap;
    PhotonMap*          _causticPhotonMap;
};

#endif // RENDERER_H
