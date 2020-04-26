#ifndef BASESYNCABLELISTENER_H
#define BASESYNCABLELISTENER_H

#include "engine/sync/syncable.h"
#include "preferences/usersettings.h"

class InternalClock;
class EngineChannel;

/// BaseSyncableListener is a superclass for receiving communication about the
/// state of the Master Sync status. It gets updates about the master bpms and
/// beat distances.  Specific subclasses could include decks, the internal clock,
/// midi devices, or other special objects that want to participate in sync.
class BaseSyncableListener : public SyncableListener {
  public:
    BaseSyncableListener(UserSettingsPointer pConfig);
    ~BaseSyncableListener() override;

    void addSyncableDeck(Syncable* pSyncable);
    EngineChannel* getMaster() const;
    void onCallbackStart(int sampleRate, int bufferSize);
    void onCallbackEnd(int sampleRate, int bufferSize);

    // Only for testing. Do not use.
    Syncable* getSyncableForGroup(const QString& group);
    Syncable* getMasterSyncable() override {
        return m_pMasterSyncable;
    };

  protected:
    // This utility method returns true if it finds a deck not in SYNC_NONE mode.
    bool syncDeckExists() const;

    // Return the current BPM of the master Syncable. If no master syncable is
    // set then returns the BPM of the internal clock.
    double masterBpm() const;

    // Returns the current beat distance of the master Syncable. If no master
    // Syncable is set, then returns the beat distance of the internal clock.
    double masterBeatDistance() const;

    // Returns the current BPM of the master Syncable if it were playing
    // at 1.0 rate.
    double masterBaseBpm() const;

    // Set the BPM on every sync-enabled Syncable except pSource.
    void setMasterBpm(Syncable* pSource, double bpm);

    // Set the master instantaneous BPM on every sync-enabled Syncable except
    // pSource.
    void setMasterInstantaneousBpm(Syncable* pSource, double bpm);

    // Set the master beat distance on every sync-enabled Syncable except
    // pSource.
    void setMasterBeatDistance(Syncable* pSource, double beat_distance);

    void setMasterParams(Syncable* pSource, double beat_distance,
                         double base_bpm, double bpm);

    // Check if there is only one playing syncable deck, and notify it if so.
    void checkUniquePlayingSyncable();

    UserSettingsPointer m_pConfig;
    // The InternalClock syncable.
    InternalClock* m_pInternalClock;
    // The current Syncable that is the master.
    Syncable* m_pMasterSyncable;
    // The list of all Syncables registered with BaseSyncableListener via
    // addSyncableDeck.
    QList<Syncable*> m_syncables;
};

#endif /* BASESYNCABLELISTENER_H */
