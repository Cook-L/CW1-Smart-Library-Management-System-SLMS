#pragma once
#include "Settings.h"
#include "Member.h"
#include "BorrowedRecord.h"
#include "ReservedRecord.h"
#include <list>

class LibrarianC : public MemberC {
public:

	LibrarianC() {
		SetRole(Librarian);
	}

	list<BorrowedRecord> ViewUserBorrows(int memberID, list<MemberC> memberList) {
		try {
			for (MemberC member : memberList) {
				if (member.GetID() == memberID) {
					return member.GetBorrowedRecords();
				}
			}

			throw invalid_argument("No Member with that ID found");
		}
		catch (exception e)
		{
			cout << "View User Borrows Function Failure!\n" << e.what();
		}
	}

	list<ReservedRecord> ViewUserReserved(int memberID, list<MemberC> memberList) {
		try {
			for (MemberC member : memberList) {
				if (member.GetID() == memberID) {
					return member.GetReservedRecordsList();
				}
			}

			throw invalid_argument("No Member with that ID found");
		}
		catch (exception e)
		{
			cout << "View User Borrows Function Failure!\n" << e.what();
		}
	}

	bool ConfirmReservation(MemberC member, int recordID) {
		try {
			for (ReservedRecord record : member.GetReservedRecordsList()) {
				if (recordID == record.GetRecordID()) {
					record.SetConfirmation(true);
					return true;
				}
			}
			throw invalid_argument("Record not found!");
		}
		catch (exception e) {
			cout << "Confirmation of Reservation Record Failure!\n" << e.what();
		}
	}

	void ConfirmBooking(MemberC member, int recordID) {
		for (BorrowedRecord record : member.GetBorrowedRecords()) {
			if (recordID == record.GetRecordID()) record.SetConfirmation(true);
		}
	}
	
};