#pragma once

#include "../GuiComponent.h"
#include "../scrapers/Scraper.h"
#include "../components/ComponentGrid.h"
#include <functional>

#define MAX_SCRAPER_RESULTS 5

class ComponentList;
class ImageComponent;
class RatingComponent;
class TextComponent;
class DateTimeComponent;
class ScrollableContainer;
class HttpReq;


class ScraperSearchComponent : public GuiComponent
{
public:
	enum SearchType
	{
		ALWAYS_ACCEPT_FIRST_RESULT,
		ALWAYS_ACCEPT_MATCHING_CRC,
		NEVER_AUTO_ACCEPT
	};

	ScraperSearchComponent(Window* window, SearchType searchType = NEVER_AUTO_ACCEPT);

	void search(const ScraperSearchParams& params);

	// Metadata assets will be resolved before calling the accept callback (e.g. result.mdl's "image" is automatically downloaded and properly set).
	inline void setAcceptCallback(const std::function<void(const ScraperSearchResult&)>& acceptCallback) { mAcceptCallback = acceptCallback; }
	inline void setSkipCallback(const std::function<void()>& skipCallback) { mSkipCallback = skipCallback; };
	inline void setCancelCallback(const std::function<void()>& cancelCallback) { mCancelCallback = cancelCallback; }

	bool input(InputConfig* config, Input input) override;
	void update(int deltaTime) override;
	void render(const Eigen::Affine3f& parentTrans) override;
	std::vector<HelpPrompt> getHelpPrompts() override;
	void onSizeChanged() override;	
	void onFocusGained() override;
	void onFocusLost() override;

private:
	void updateViewStyle();
	void updateThumbnail();
	void updateInfoPane();

	void onSearchError(const std::string& error);
	void onSearchDone(const std::vector<ScraperSearchResult>& results);

	int getSelectedIndex();

	// resolve any metadata assets that need to be downloaded and return
	void returnResult(ScraperSearchResult result);

	ComponentGrid mGrid;

	std::shared_ptr<TextComponent> mResultName;
	std::shared_ptr<ScrollableContainer> mDescContainer;
	std::shared_ptr<TextComponent> mResultDesc;
	std::shared_ptr<ImageComponent> mResultThumbnail;
	std::shared_ptr<ComponentList> mResultList;

	std::shared_ptr<ComponentGrid> mMD_Grid;
	std::shared_ptr<RatingComponent> mMD_Rating;
	std::shared_ptr<DateTimeComponent> mMD_ReleaseDate;
	std::shared_ptr<TextComponent> mMD_Developer;
	std::shared_ptr<TextComponent> mMD_Publisher;
	std::shared_ptr<TextComponent> mMD_Genre;
	std::shared_ptr<TextComponent> mMD_Players;

	// label-component pair
	typedef std::pair< std::shared_ptr<TextComponent>, std::shared_ptr<GuiComponent> > MetaDataPair;
	std::vector<MetaDataPair> mMD_Pairs;

	SearchType mSearchType;
	ScraperSearchParams mLastSearch;
	std::function<void(const ScraperSearchResult&)> mAcceptCallback;
	std::function<void()> mSkipCallback;
	std::function<void()> mCancelCallback;
	bool mBlockAccept;

	std::unique_ptr<ScraperSearchHandle> mSearchHandle;
	std::unique_ptr<MDResolveHandle> mMDResolveHandle;
	std::vector<ScraperSearchResult> mScraperResults;
	std::unique_ptr<HttpReq> mThumbnailReq;
};