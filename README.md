# anagrams
This is a small project for finding anagrams of words.

It works by first sorting the letters in the target word into alphabetical order and then searching for matching sorted words in a preprepared list of words and their sorted counterparts.

It comes packaged with a large list of English words and has the capability to preprocess and use other lists of words.

### Usage
- To search for anagrams: `anagrams word1 word2 ...`
- To preprocess a new list of words: `anagrams --generateWordList file`
