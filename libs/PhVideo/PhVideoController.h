#ifndef PHVIDEOCONTROLLER_H
#define PHVIDEOCONTROLLER_H

#include <QString>
#include <QMediaPlayer>

#include "PhVideoView.h"

/**
 * This class allow to handle the video media.
 * It also create the view to display it.
 */
class PhVideoController : public QMediaPlayer
{
    Q_OBJECT
public:
    /**
     * @brief PhVideoController constructor
     * @param parent widget for the controller
     */
    explicit PhVideoController(QObject *parent = 0);
    
    /**
     * @brief Get the view associated with the controller
     * @return A PhVideoView instance
     */
    PhVideoView * getView();

    /**
     * @brief Open a video file
     * @param fileName A video file path
     * @return True if the file was opened successfully, false otherwise
     */
    bool open(QString fileName);
signals:
    
public slots:
    void playPause();

private:
    PhVideoView _videoView;
};

#endif // PHVIDEOCONTROLLER_H
