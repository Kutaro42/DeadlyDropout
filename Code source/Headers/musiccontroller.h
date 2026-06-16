/**
 * @file musiccontroller.h
 * @brief Singleton gérant la lecture musicale (menu et in-game).
 */
#ifndef MUSICCONTROLLER_H
#define MUSICCONTROLLER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStringList>
#include <QRandomGenerator>
#include <QCoreApplication>

class MusicController : public QObject
{
    Q_OBJECT
public:
    static MusicController& instance() {
        static MusicController inst;
        return inst;
    }

    /** Démarre la musique du menu (en boucle). */
    void playMenu();

    /** Arrête toute musique en cours. */
    void stop();

    /** Lance une piste in-game aléatoire. */
    void playRandomGame();

    /** Met en pause / reprend la piste courante. */
    void togglePause();

    /** Passe à la prochaine piste aléatoire in-game. */
    void nextTrack();

    bool isPlaying() const;

private:
    explicit MusicController(QObject* parent = nullptr);
    ~MusicController() = default;
    MusicController(const MusicController&)            = delete;
    MusicController& operator=(const MusicController&) = delete;

    void playTrack(const QString& resourcePath);

    QMediaPlayer* player;
    QAudioOutput* audioOutput;

    QStringList   gameTracks;   ///< Liste des chemins Qt resource des pistes in-game.
    int           lastIndex;    ///< Dernier index joué (pour éviter répétition immédiate).
};

#endif // MUSICCONTROLLER_H
