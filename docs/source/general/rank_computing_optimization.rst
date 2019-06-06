Rank computation optimization
=============================

How do we optimize rank and top list computation?
-------------------------------------------------

The recommender models implement different interfaces based on what type of output can they easily generate.  Examples: a popularity based model can easily generate a top list.  A matrix factorization based model provides a score for each user-item pair. Using a :py:class:`alpenglow.cpp.LempContainer`, the same model can efficiently serve the items roughly sorted descending by score, that lets us optimize rank computation.

Available interfaces and tools
------------------------------

1. :py:class:`alpenglow.cpp.ToplistRecommender` interface -> returns filtered toplist of items for user.
2. :py:class:`alpenglow.cpp.RankingScoreIteratorProvider` interface -> implementing class provides a :py:class:`alpenglow.cpp.RankingScoreIterator` for itself. The iterator lets the rank computer or toplist creator iterate on the items in a roughly score-descending order.
3. :py:class:`alpenglow.cpp.ModelFilter` interface -> DEPRECATED for this type of usage in favor of :py:class:`alpenglow.cpp.RankingScoreIterator`.  Has similar functionality + can filter out some items.
4. :py:class:`alpenglow.cpp.Model` -> original, default interface.  Returns score.

Rank computation methods
------------------------

Using the interfaces listed above:

1. get the toplist, find the active item.
2. get the RankingScoreIterator, iterate on items score descending. Count items that have higher score than current one.  Break computation if the score of remaining items is lower than the score of the current item.
3. like the previous one
4. iterate on the items (sorted by popularity, that correlates with the score to some extent). Count items that have higher score than the current item.  Break cycle if found top_k items having higher score than the current item, because in that case the current item is not included in the top list.

Top list computation methods
----------------------------

Using the interfaces listed above:

1. get the toplist.
2. get the RankingScoreIterator, iterate on items score descending.  Use a fixed size heap to keep an up-to-date list of items having the highest score.  No more items having higher score than heap.min -> the toplist is the heap in a reverse order.  This is impelemented in :py:class:`alpenglow.cpp.ToplistFromRankingScoreRecommender`.
3. like the previous one, but no reusable implementation
4. trivial brute-force algorithm


