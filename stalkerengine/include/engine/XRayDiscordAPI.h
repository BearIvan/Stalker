struct DiscordUser;
class XRayDiscordAPI:public pureFrame
{ 
public:
	BEAR_CLASS_NO_COPY(XRayDiscordAPI)
	XRayDiscordAPI();
	~XRayDiscordAPI();
	virtual void OnFrame();
protected:
	static void Ready(const DiscordUser* request);
	static void Disconnected(int errorCode, const char* message);
	static void Errored(int errorCode, const char* message);
	static void JoinGame(const char* joinSecret);
	static void SpectateGame(const char* spectateSecret);
	static void JoinRequest (const DiscordUser* request);
private:
	int64_t m_StartTime;
};