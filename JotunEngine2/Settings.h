#pragma once
/*!\brief Holds a number of quality settings for the traditional accuracy vs speed tradeoff*/
class Settings {
public:
	/*!\brief Allows the user to set shadow quality
	
	/param [in] quality The desired quality of the shadows
	0 = no shadows
	1 = hard shadows
	2 = soft shadows
	3 = PCSS shadows (prettiest)*/
	static void setShadowQuality( int quality );
	/*!\brief Returns the current shadow quality
	
	\return The current shadow quality*/
	static int	getShadowQuality();
protected:
	static int	shadowQuality;
};

