#pragma once

#include <string>
#include <stdint.h>

class CSelfUpdater
{
public:

	struct stVersion {
		uint8_t major;
		uint16_t minor;
		uint8_t patch;

		bool operator==(const stVersion& rhs) const {
			return (rhs.major == major && rhs.minor == minor && rhs.patch == patch);
		}

		bool operator!=(const stVersion& rhs) const {
			return !(*this == rhs);
		}

		bool operator>(const stVersion& rhs) const {
			if (major > rhs.major) return 1;
			else if (major == rhs.major)
			{
				if (minor > rhs.minor) return 1;
				else if (minor == rhs.minor)
				{
					if (patch > rhs.patch) return 1;	
				}
			}
			return 0;
		}

		bool operator<(const stVersion& rhs) const {
			return (*this != rhs && !(*this > rhs));
		}

		bool operator>=(const stVersion& rhs) const {
			return (*this > rhs || *this == rhs);
		}

		bool operator<=(const stVersion& rhs) const {
			return (*this < rhs || *this == rhs);
		}
	};

	CSelfUpdater(stVersion currVersion, std::string url);
	~CSelfUpdater();

	bool CheckForNewVersion();
	bool DownloadUpdate();
	bool ApplyUpdate();

	stVersion m_currentVersion;
	stVersion m_newVersion;
	std::string m_fileURL;
	std::string m_fileMD5;
	std::string m_additionalInfo;

private:
	bool GetModulePath(std::string& path);
	bool GeneratePaths();

	std::string m_updateInfoURL;
	std::string m_newVerTempFilePath;
	std::string m_currentFilePath;
	std::string m_oldVerTempFilePath;
	std::string m_parentPath;
};