/******************************
* Qt player using libVLC     *
* By protonux                *
*                            *
* Under WTFPL                *
******************************/

#ifndef PLAYER
#define PLAYER

#include <QtGui>
#include <vlc/vlc.h>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>

class Mwindow : public QMainWindow {

	Q_OBJECT

public:
	Mwindow();
	virtual ~Mwindow();
	void openFile(QString fileName);

private slots:
	void openFile();
	void play();
	void stop();
	void mute();
	void about();
	void fullscreen();

	int changeVolume(int);
	void changePosition(int);
	void updateInterface();

protected:
	virtual void closeEvent(QCloseEvent*);

private:
	QPushButton *playBut;
	QSlider *volumeSlider;
	QSlider *slider;
	QWidget *videoWidget;

	libvlc_instance_t *vlcInstance;
	libvlc_media_player_t *vlcPlayer;

	void initUI();
};



#endif
