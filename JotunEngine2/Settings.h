#pragma once
class Settings {
public:
	static void setShadowQuality( int quality );
	static int	getShadowQuality();
protected:
	static int	shadowQuality;
};

