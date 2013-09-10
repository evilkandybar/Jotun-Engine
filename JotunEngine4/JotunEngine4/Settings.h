class Settings {
public:
	static int msaaSamples;		/*!< The number of desired multisampled antialiasing samples*/
	static int anisoSamples;	/*!< The number of anisotropic texture filtering samples. */
	static bool trilinear;		/*!< If true, trilinear filtering is enabled and anisaSampes works as expected.
									 If false, bilinear filtering is enabled and anisoSampes is irrelevant*/
};