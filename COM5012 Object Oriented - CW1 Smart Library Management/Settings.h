#pragma once

static class Settings {
private:
	static float latePenalty;
	static int borrowingLimit;
	static int daysAfterBorrowing;
public:
	static float GetLatePenalty() { return latePenalty; }
	static float GetBorrowingLimit() { return borrowingLimit; }
	static int GetBorrowingDays() { return daysAfterBorrowing; }
	static bool SetLatePenalty(float newLatePenalty) { latePenalty = newLatePenalty; }
	static bool SetBorrowingLimit(float newBorrowingLimit) { borrowingLimit = newBorrowingLimit; }
	static bool SetBorrowingDays(int days) { daysAfterBorrowing = days; }
};