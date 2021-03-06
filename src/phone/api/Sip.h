/****************************************************************************
**
** Copyright (C) 2012 Lorem Ipsum Mediengesellschaft m.b.H.
**
** GNU General Public License
** This file may be used under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation and
** appearing in the file LICENSE.GPL included in the packaging of this file.
**
****************************************************************************/

#ifndef SIPPHONE_INCLUDE_H
#define SIPPHONE_INCLUDE_H

#include <pjsua-lib/pjsua.h>
#include <QVector>
#include "Interface.h"

namespace phone
{
    class Phone;
    class Settings;

    namespace api
    {

/**
 * This class is an implementation of PhoneApi for sip-Protocol
 */
class Sip : public Interface
{
public:
    Sip();
    ~Sip();

    virtual bool init(const Settings &settings);

    virtual int registerUser(const QString &user, const QString &password, const QString &domain);
    virtual bool checkAccountStatus();
    virtual void unregister();
    virtual void getAccountInfo(QVariantMap &account_info);

    virtual int makeCall(const QString &url);
    virtual int makeCall(const QString &url, const QVariantMap &header_map);
    virtual void answerCall(int call_id = -1, int code = 200);
    virtual void hangUp(const int call_id);
    virtual void hangUpAll();

    virtual bool addCallToConference(const int call_src, const int call_dest);
    virtual bool removeCallFromConference(const int call_src, const int call_dest);

    virtual int redirectCall(const int call_id, const QString &dest_uri);

    virtual void getCallInfo(const int call_id, QVariantMap &call_info);

    virtual void setSoundSignal(const float soundLevel, const int call_id = -1);
    virtual void setMicroSignal(const float microLevel, const int call_id = -1);
    virtual void getSignalLevels(QVariantMap &levels, const int call_id = -1);
    
    /**
     * Set priority of codec to new_priority
     * Codecs examples (priority: codec name):
     * 130: speex/16000/1
     * 129: speex/8000/1
     * 128: speex/32000/1
     * 128: iLBC/8000/1
     * 128: GSM/8000/1
     * 128: PCMU/8000/1
     * 128: PCMA/8000/1
     * 128: G722/16000/1
     * 0: L16/44100/1
     * 0: L16/44100/2
     * 0: L16/8000/1
     * 0: L16/8000/2
     * 0: L16/16000/1
     * 0: L16/16000/2
     * @param codec Name of Codec
     * @param new_priority Range 0...PJMEDIA_CODEC_PRIO_HIGHEST
     */
    virtual void setCodecPriority(const QString &codec, int new_priority);
    virtual void getCodecPriorities(QVariantMap &codecs);
    
    virtual bool setSoundDevice(const int input, const int output);
    virtual void getSoundDevices(QVariantList &device_list);
    
    virtual bool sendDTMFDigits(int call_id, const QString &digits);

    /**
     * Get the sip-address of a given call
     * @param call_id int, the id of the call
     * @return QString the sip-address
     */
    QString getCallUrl(const int call_id);
    
private:
    /**
     * Pointer to the instance for using static methods
     */
    static Sip *self_;

    /**
     * AccountID we got from our registration
     */
    pjsua_acc_id account_id_;

    /**
     * Initialize pjsua
     * @return true, if successful
     */
    bool _initPjsua(const Settings &settings);

    /**
     * Add UDP transport
     * @param type Transport type
     * @param port
     * @return true, if successful
     */
    bool _addTransport(pjsip_transport_type_e type, unsigned int port);

    /**
     * PJSIP callback for incoming calls
     * @param acc_id The id of the account receiving the incoming call
     * @param call_id The id of the new incoming call
     * @param rdata Incoming message buffer
     */
    static void incomingCallCb(pjsua_acc_id acc_id, pjsua_call_id call_id,
                               pjsip_rx_data *rdata);

    /**
     * PJSIP callback for call state changes
     * @param call_id The id of the call which changed
     * @param e PJSIP event
     */
    static void callStateCb(pjsua_call_id call_id, pjsip_event *e);

    /**
     * PJSIP callback for media state changes
     * @param call_id The id of the call which changed
     */
    static void callMediaStateCb(pjsua_call_id call_id);

    /**
     * PJSIP callback for reg_state changes
     * @param acc The id of the account which changed
     */
    static void registerStateCb(pjsua_acc_id acc);

    /**
     * Callback for incoming text messages
     * @param call_id Containts the ID of the call where the IM was sent, or PJSUA_INVALID_ID if the IM was sent outside call context.
     * @param from URI of the sender.
     * @param to URI of the destination message.
     * @param contact The Contact URI of the sender, if present.
     * @param mime_type MIME type of the message.
     * @param body The message content.
     */
    static void incomingTextMessageCb(pjsua_call_id call_id, const pj_str_t *from, const pj_str_t *to, const pj_str_t *contact, const pj_str_t *mime_type, const pj_str_t *body);
};

}} // phone::api::

#endif // SIPPHONE_INCLUDE_H
