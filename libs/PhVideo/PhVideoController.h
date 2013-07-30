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
    PhVideoView * view();

    /**
     * @brief Open a video file
     * @param fileName A video file path
     * @return True if the file was opened successfully, false otherwise
     */
    bool open(QString fileName);
signals:
    /**
     * Set the position according to the duration:
     * 0 for the beginning
     * 100 for the end
     * @param percentage position in percentage.
     */
    void positionPercentageChanged(int percentage);

public slots:
    /**
     * Play if the video is in pause and pause otherwise
     */
    void playPause();

    void updatePositionFromPercentage(int percentage);

private:
    PhVideoView _videoView;

private slots:
    /**
     * Update the percentage when the media position changes.
     * @param position Position of the media.
     */
    void updatePercentageFromPosition(qint64 position);
};

#endif // PHVIDEOCONTROLLER_H
