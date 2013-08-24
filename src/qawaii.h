#ifndef QAWAII_H
#define QAWAII_H

#include <QDialog>
#include <QtGui>

#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>

struct scrobbler_queue_t
{
	unsigned long int timestamp;
	QString artist;
	QString title;
};

class Qawaii: public QDialog
{
	Q_OBJECT
	public:
		Qawaii(QWidget *parent = 0, Qt::WFlags f = 0);
		~Qawaii();
	
	public slots:
		void setSource();
		void metaDataChanged();
		void stateChanded(Phonon::State newstate, Phonon::State oldstate);
		void iconClicked(QSystemTrayIcon::ActivationReason reason);
	
	private:
		QSystemTrayIcon *trayicon;
		QLineEdit *source_edit;
		QPushButton *set_source_button;
		QLabel *status_label;
		QLabel *radio_title_label; // название радиостанции
		QLabel *artist_label; // исполнитель
		QLabel *song_label; // название композиции
		QProgressBar *buffer_status;
		Phonon::MediaObject *media_object;
		Phonon::AudioOutput *audio_output;
		Phonon::Path *audio_output_path;
		Phonon::VolumeSlider *volume_slider;
		QList<scrobbler_queue_t> scrobbler_queue;
		void addToQueue(QString artist, QString title);
		void setStatus(char *text);
		Phonon::State player_state;
	
	private slots:
		void createObjects();
		void createConnections();
};

#endif
