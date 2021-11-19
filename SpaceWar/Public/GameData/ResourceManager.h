#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>

enum class EResourceTag : uint8_t
{
	PlayerTexture,
	EnemyTexture,
	BackGround,
	Projectile,
	Particle,
	MAX
};

template<typename Derived, typename T>
class ResourceManager 
{
	std::unordered_map<EResourceTag, std::pair<std::unique_ptr<T>, unsigned int>> mResources;

	std::pair<std::unique_ptr<T>, unsigned int>* Find(const EResourceTag& Tag) 
	{
		auto ResourceItr = mResources.find(Tag);
		return (ResourceItr != mResources.end() ? &ResourceItr->second : nullptr);
	}

	bool Unload(const EResourceTag& Id) {
		auto ResourceItr = mResources.find(Id);
		if (ResourceItr == mResources.end())
		{ 
			return false; 
		}
		ResourceItr->second.get();
		mResources.erase(ResourceItr);
		return true;
	}

public:

	ResourceManager() {}

	virtual ~ResourceManager() 
	{ 
		PurgeResources(); 
	}

	bool LoadResource(const EResourceTag& Tag, const std::string& Resource) {
		auto res = Find(Tag);
		if (res) {
			++res->second;
			return true;
		}
		std::unique_ptr<T> resource = Load(Resource);
		if (!resource) 
		{ 
			return false; 
		}
		mResources.emplace(Tag, std::make_pair(std::move(resource),1));
		return true;
	}

	const T& GetResource(const EResourceTag& Tag) {
		auto Resource = Find(Tag);
		return *Resource->first;
	}


	bool ReleaseResource(const EResourceTag& Tag) {
		auto Resource = Find(Tag);
		if (!Resource) 
		{ 
			return false; 
		}
		--Resource->second;
		if (!Resource->second) 
		{ 
			Unload(Tag);
		}
		return true;
	}

	void PurgeResources() {
		while (mResources.begin() != mResources.end()) {
			mResources.erase(mResources.begin());
		}
	}

protected:
	std::unique_ptr<T> Load(const std::string& Path) {
		return static_cast<Derived*>(this)->Load(Path);
	}
};

class ResourceException : public std::exception
{
public:
	explicit ResourceException(const char* message)
		: mMsg(message) {}


	explicit ResourceException(const std::string& message)
		: mMsg(message) {}

	virtual ~ResourceException() noexcept {}
	
	virtual const char* what() const noexcept {
		return mMsg.c_str();
	}

protected:
	std::string mMsg;
};