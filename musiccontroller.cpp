/**
 * @file musiccontroller.cpp
 * @brief Implémentation du MusicController singleton.
 */
#include "musiccontroller.h"
#include <QUrl>

MusicController::MusicController(QObject* parent)
    : QObject(parent)
    , player(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
    , lastIndex(-1)
{
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.2f); // 50% — ajustez selon vos besoins

    // ── Liste des pistes in-game ──────────────────────────────────
    // Adaptez ces chemins à vos ressources dans le .qrc
    gameTracks << "qrc:/sounds/music/track1.mp3"
               << "qrc:/sounds/music/track2.mp3"
               << "qrc:/sounds/music/track3.mp3"
               << "qrc:/sounds/music/chad.wav";
}

void MusicController::playTrack(const QString& resourcePath)
{
    player->setSource(QUrl(resourcePath));
    player->setLoops(QMediaPlayer::Infinite);
    player->play();
}

void MusicController::playMenu()
{
    // Adaptez ce chemin à votre musique de menu dans le .qrc
    playTrack("qrc:/sounds/music/menu.mp3");
}

void MusicController::stop()
{
    player->stop();
}

void MusicController::playRandomGame()
{
    if (gameTracks.isEmpty()) return;

    int index;
    if (gameTracks.size() == 1) {
        index = 0;
    } else {
        do {
            index = static_cast<int>(
                QRandomGenerator::global()->bounded(gameTracks.size()));
        } while (index == lastIndex);
    }
    lastIndex = index;
    playTrack(gameTracks[index]);
}

void MusicController::togglePause()
{
    if (player->playbackState() == QMediaPlayer::PlayingState)
        player->pause();
    else
        player->play();
}

void MusicController::nextTrack()
{
    playRandomGame();
}

bool MusicController::isPlaying() const
{
    return player->playbackState() == QMediaPlayer::PlayingState;
}
