#pragma once

#include <FuzzySearch.h>

Q_DECLARE_METATYPE(FuzzySearch::PatternMatch)

namespace HistoryViewConstants
{
const int DISPLAY_STRING_MAX_LENGTH = 80;
const int HISTORY_ITEM_FILTER_MATCH = Qt::UserRole + 1;
} // namespace HistoryViewConstants