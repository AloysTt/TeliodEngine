namespace teliod::physics
{
	inline bool AlmostEqualRelativeAndAbs(float A, float B, float maxDiff, float maxRelDiff) {
		float diff = fabs(A - B);
		if (diff <= maxDiff) {
			return true;
		}

		A = fabs(A);
		B = fabs(B);
		float largest = (B > A) ? B : A;

		if (diff <= largest * maxRelDiff) {
			return true;
		}
		return false;
	}
}
