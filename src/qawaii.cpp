
#include "qawaii.h"

Qawaii::Qawaii(QWidget *parent, Qt::WFlags f): QDialog(parent, f) {
	audio_output = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	media_object = new Phonon::MediaObject(this);

	createObjects();
	createConnections();

	Phonon::createPath(media_object, audio_output);
	setStatus("Qawaii is ready!");
	player_state = Phonon::StoppedState;
}

Qawaii::~Qawaii() {
	delete media_object;
	delete audio_output;
}

void Qawaii::createConnections() {
	connect(set_source_button, SIGNAL(clicked()), this, SLOT(setSource()));
	connect(media_object, SIGNAL(bufferStatus(int)), buffer_status, SLOT(setValue(int)));
	connect(media_object, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
	connect(media_object, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanded(Phonon::State, Phonon::State)));
	
	connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconClicked(QSystemTrayIcon::ActivationReason)));
}

void Qawaii::iconClicked(QSystemTrayIcon::ActivationReason reason) {
	if(reason == QSystemTrayIcon::Trigger) {
		if(isVisible()) setVisible(false);
		else setVisible(true);
	}
}

void Qawaii::createObjects() {
	this->resize(300, 50);
	
	trayicon = new QSystemTrayIcon(QIcon(":/res/qawaii.svg"));
	trayicon->setVisible(true);
	
	QHBoxLayout *source = new QHBoxLayout;
		//source_edit = new QLineEdit("http://dialup.kawaii-radio.net/"); // Kawaii radio (Japan) // dead!
		source_edit = new QLineEdit("http://175.103.49.43:8000/");
		set_source_button = new QPushButton("Play");
		source->addWidget(source_edit);
		source->addWidget(set_source_button);
	
    QHBoxLayout *info_volume = new QHBoxLayout;
		status_label = new QLabel("");
		volume_slider = new Phonon::VolumeSlider(audio_output);
		info_volume->addWidget(status_label);
		info_volume->addStretch(1);
		info_volume->addWidget(volume_slider);
	
	QGridLayout *song_info = new QGridLayout;
		radio_title_label = new QLabel("");
		artist_label = new QLabel("");
		song_label = new QLabel("");
		song_info->addWidget(new QLabel("Station:"), 0, 0, Qt::AlignLeft);
		song_info->addWidget(new QLabel("Artist:"), 1, 0, Qt::AlignLeft);
		song_info->addWidget(new QLabel("Song:"), 2, 0, Qt::AlignLeft);
		song_info->addWidget(radio_title_label, 0, 1, Qt::AlignLeft);
		song_info->addWidget(artist_label, 1, 1, Qt::AlignLeft);
		song_info->addWidget(song_label, 2, 1, Qt::AlignLeft);
		song_info->setColumnStretch(1, 1);

	QVBoxLayout *layout = new QVBoxLayout;
		buffer_status = new QProgressBar;
		buffer_status->setRange(0, 100);
		layout->addLayout(source);
		layout->addLayout(info_volume);
		layout->addLayout(song_info);
		layout->addWidget(buffer_status);
		layout->addStretch(1);
	
	this->setLayout(layout);
	
	qDebug() << "> createObjects()";
}

void Qawaii::setSource() {
	if(player_state != Phonon::StoppedState) {
		media_object->stop();
	}
	if(!source_edit->text().isEmpty()) {
		media_object->setCurrentSource(Phonon::MediaSource(QUrl::fromEncoded(source_edit->text().toUtf8())));
		media_object->play();
		set_source_button->setText("Stop");
	}
}

void Qawaii::addToQueue(QString artist, QString title) {
	QString message_title = QString("Now playing");
	QString message_text = QString(artist + QString::fromUtf8(" — ") + title);
	trayicon->showMessage(message_title, message_text);
	artist_label->setText(artist);
	song_label->setText(title);
	scrobbler_queue_t current;
	current.artist = artist;
	current.title = title;
	current.timestamp = 0;
	scrobbler_queue << current;
}

void Qawaii::metaDataChanged() {
	if(player_state == Phonon::StoppedState) { // Название радио
		radio_title_label->setText(media_object->metaData().value("TITLE"));
		return;
	}
	QString artist = media_object->metaData().value("ARTIST");
	QString title = media_object->metaData().value("TITLE");
	if(artist.isEmpty()) {
		QRegExp format = QRegExp("^([^\\-]+) - ([^\\(]+)(\\(.*\\))*$");
		if(format.exactMatch(title)) {
			QStringList song_info = format.capturedTexts();
			addToQueue(song_info[1].trimmed(), song_info[2].trimmed());
		} else {
			qDebug() << title;
		}
	} else {
		addToQueue(artist.trimmed(), title.trimmed());
	}
}

void Qawaii::setStatus(char *text) {
	status_label->setText(QString::fromUtf8(text));
}

void Qawaii::stateChanded(Phonon::State newstate, Phonon::State oldstate) {
	player_state = newstate;
	switch(newstate) {
		case Phonon::StoppedState: // выключено
			setStatus("Radio stopped.");
		break;
		case Phonon::LoadingState: // подключение
			setStatus("Connecting…");
		break;
		case Phonon::BufferingState: // буферизация
			setStatus("Buffering…");
		break;
		case Phonon::PlayingState: // воспроизведение
			setStatus("Playing stream");
		break;
		case Phonon::ErrorState: // ошибка
			setStatus("Error!");
		break;
	}
}
