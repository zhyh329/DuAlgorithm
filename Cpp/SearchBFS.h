#pragma once
#include "common.h"

namespace BFS {
	int minStickers(vector<string>& stickers, string target) {
		using Freq = vector<int>;
		const int MAX_ALPHABET = 26;
		vector<Freq> freqs(stickers.size());

		// calc frequency statistics
		auto buildFreqFromString = [=](const string &s) {
			Freq ans(MAX_ALPHABET);
			for (const auto &c : s) {
				++ans[c - 'a'];
			}
			return ans;
		};
		Freq targetFreq = buildFreqFromString(target);
		for (int i = 0; i < stickers.size(); ++i) {
			freqs[i] = buildFreqFromString(stickers[i]);
			// remove higher frequency than the target
			for (int j = 0; j < MAX_ALPHABET; ++j) {
				freqs[i][j] = min(freqs[i][j], targetFreq[j]);
			}
		}


		// test if a is the proper subset of b
		auto isProperSubset = [=](const Freq &a, const Freq &b) {
			for (int i = 0; i < a.size(); ++i) {
				if (a[i] >= b[i]) return false;
			}
			return true;
		};
		// remove the proper subset of stickets
		for (int i = (int)freqs.size() - 1; i >= 0; --i) {
			for (int j = 0; j < (int)freqs.size(); ++j) {
				if (i != j && isProperSubset(freqs[i], freqs[j])) {
					freqs.erase(freqs.begin() + i);
				}
			}
		}

		using Item = pair<int, Freq>;
		// q stores the current step and current goal
		// the default queue
		queue<Item*> q;
		unordered_map<string, bool> vd;
		q.push(new Item(0, targetFreq));
		vd[target] = true;

		auto hasOverlap = [=](const Freq &a, const Freq &b) {
			for (int i = 0; i < a.size(); ++i) {
				if (a[i] > 0 && b[i] > 0) return true;
			}
			return false;
		};

		auto subtractFreq = [=](const Freq &a, const Freq &b) {
			Freq ans(26);
			for (int i = 0; i < a.size(); ++i) {
				ans[i] = max(0, a[i] - b[i]);
			}
			return ans;
		};

		auto getStringFromFreq = [=](const Freq &a) {
			string ans = "";
			for (int i = 0; i < a.size(); ++i) {
				ans += string(char(int('a') + i), a[i]);
			}
			return ans;
		};

		auto isEmpty = [=](const Freq &a) {
			for (int i = 0; i < a.size(); ++i)
				if (a[i] > 0) return false;
			return true;
		};

		// BFS
		while (!q.empty()) {
			auto current = q.front();
			q.pop();
			for (auto &freq : freqs) {
				if (!hasOverlap(current->second, freq))
					continue;
				auto next = subtractFreq(current->second, freq);
				auto next_str = getStringFromFreq(next);
				if (isEmpty(next)) return current->first + 1;
				if (vd.find(next_str) == vd.end()) {
					q.push(new Item(current->first + 1, next));
					vd[next_str] = true;
				}
			}
			// we should delete the pointers, but not this time
			// other solutions are to use vectors instead of queue
			// and remove the vector in the end
			// delete current;
		}
		return -1;
	}

	int kSimilarity(string A, string B) {
		queue<string> q;
		unordered_set<string> vd;

		vd.emplace(A);
		q.emplace(A);
		int res = 0;

		while (!q.empty()) {
			for (int size = (int)q.size() - 1; size >= 0; --size) {
				auto s = q.front();
				q.pop();
				if (s == B) {
					return res;
				}
				int i;
				for (i = 0; s[i] == B[i]; ++i);
				for (int j = i + 1; j < s.size(); ++j) {
					if (s[j] == B[j] || s[i] != B[j]) {
						continue;
					}
					swap(s[i], s[j]);
					if (!vd.count(s)) {
						vd.emplace(s);
						q.emplace(s);
					}
					swap(s[i], s[j]);
				}
			}
			++res;
		}
		return res;
	}

	inline bool valid(int y, int x, int n, int m) {
		return y >= 0 && y <= n - 1 && x >= 0 && x <= m - 1;
	}

	int minArea(vector<vector<char>>& image, int y, int x) {
		const auto n = image.size();
		const auto m = n ? image[0].size() : 0;
		if (!valid(y, x, (int)n, (int)m) || image[y][x] != '1') return 0;

		int ans = 0;
		using CT = pair<int, int>;
		vector<vector<bool>> vd(n, vector<bool>(m));
		CT start = CT(y, x);
		CT topleft = start, botright = start;
		vd[y][x] = true;
		queue<CT> q;
		q.push(start);
		vector<vector<int>> dir{ { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };

		while (!q.empty()) {
			int i = q.front().first;
			int j = q.front().second;
			q.pop();
			for (int k = 0; k < dir.size(); ++k) {
				int r = i + dir[k][0];
				int c = j + dir[k][1];
				if (!valid(r, c, (int)n, (int)m) || vd[r][c] || image[r][c] == '0') continue;
				vd[r][c] = true;
				CT newc = CT(r, c);
				if (r < topleft.first) topleft.first = r;
				if (c < topleft.second) topleft.second = c;
				if (r > botright.first) botright.first = r;
				if (c > botright.second) botright.second = c;
				q.push(newc);
			}
		}
		return (botright.first - topleft.first + 1) * (botright.second - topleft.second + 1);
	}

	// 126. Word Ladder II
	// Given two words (beginWord and endWord), and a dictionary's word list, find all shortest transformation sequence(s) from beginWord to endWord, such that:
	/*
		Only one letter can be changed at a time
		Each transformed word must exist in the word list.Note that beginWord is not a transformed word.
		Note:

	Return an empty list if there is no such transformation sequence.
		All words have the same length.
		All words contain only lowercase alphabetic characters.
		You may assume no duplicates in the word list.
		You may assume beginWord and endWord are non - empty and are not the same.
	*/
	// BFS + DFS
	vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
		vector<vector<string>> res;
		wordList.emplace_back(beginWord);

		const int n = (int)wordList.size();
		int start = -1, target = -1;
		vector<vector<int>> edge(n, vector<int>());

		auto isConnected = [=](const string& a, const string& b) {
			if (a.size() != b.size())
				return false;
			int cnt = 0;
			for (int i = 0; i < a.size(); ++i) {
				if (a[i] != b[i])
					++cnt;
				if (cnt > 1)
					return false;
			}
			return cnt == 1;
		};

		for (int i = 0; i < n; ++i) {
			if (!wordList[i].compare(beginWord)) {
				start = i;
			}
			if (!wordList[i].compare(endWord)) {
				target = i;
			}
			for (int j = i + 1; j < n; ++j) {
				if (isConnected(wordList[i], wordList[j])) {
					edge[i].emplace_back(j);
					edge[j].emplace_back(i);
					//cout << "c" << i << " " << j << endl;
				}
			}
		}
		if (start == -1 || target == -1)
			return res;

		queue<pair<int, int>> q;
		q.emplace(make_pair(target, 0));

		vector<bool> vd(n, false);
		vector<bool> marked(n, false);

		vd[target] = true;
		marked[target] = true;

		vector<vector<int>> path(n, vector<int>());
		int current_layer = 0;
		vector<int> next_layer;
		bool finish = false;

		//cout << start << " " << target << endl;

		while (!q.empty()) {
			auto p = q.front();
			q.pop();
			int i = p.first;
			int layer = p.second;
			cout << i << " " << layer << endl;

			if (layer != current_layer) {
				if (finish) {
					cout << "finish" << endl;
					vector<string> ans;
					stack<pair<int, int>> DFS;
					DFS.push({ start, 0 });
					while (!DFS.empty()) {
						auto p = DFS.top();
						auto i = p.first;
						auto layer = p.second;
						DFS.pop();
						while (ans.size() > layer) {
							ans.pop_back();
						}
						ans.emplace_back(wordList[i]);

						if (i != target) {
							for (const auto j : path[i]) {
								DFS.push({ j, layer + 1 });
							}
						}
						else {
							auto copied = ans;
							res.emplace_back(copied);
						}
					}
					return res;
				}

				current_layer = layer;
				for (auto j : next_layer) {
					marked[j] = true;
				}
				next_layer.clear();
			}
			for (const auto j : edge[i]) {
				if (j == start) {
					finish = true;
				}
				if (!vd[j]) {
					q.emplace(make_pair(j, layer + 1));
					vd[j] = true;
				}
				if (!marked[j]) {
					path[j].emplace_back(i);
					next_layer.emplace_back(j);
				}
			}
		}
		return res;
	}
}