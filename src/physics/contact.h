#pragma once


namespace Construct
{
	struct Contact
	{
		Body* body1;
		Body* body2;
		Vector2f witness1;
		Vector2f witness2;
		Vector2f normal;
		float distance;
	};
}
