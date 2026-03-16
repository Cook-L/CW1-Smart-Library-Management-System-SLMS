#pragma once

static class Settings {
private:
	static float latePenalty;
	static int borrowingLimit;
public:
	static float GetLatePenalty() { return latePenalty; }
	static float GetBorrowingLimit() { return borrowingLimit; }
	static bool SetLatePenalty(float newLatePenalty) { latePenalty = newLatePenalty; }
	static bool SetBorrowingLimit(float newBorrowingLimit) { borrowingLimit = newBorrowingLimit; }
};