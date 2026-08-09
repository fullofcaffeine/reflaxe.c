package caxecraft.app;

/**
 * Gives gameplay and presentation the small read-only item view they share.
 *
 * The runtime content generation owns richer authored IDs and quantities.
 * Caxecraft's current fixed item slots need only the resolved storage code and
 * placement for pickup tests and drawing, so the application copies those
 * scalars once at startup instead of retaining a mutable content-plan Array.
 */
typedef LoadedWorldItem = {
	/** Pack-local item code already validated by the selected content registry. */
	final storageCode:Int;

	/** Complete stack quantity that enters inventory in one pickup transaction. */
	final quantity:Int;

	/** Horizontal placement in thousandths of one world block. */
	final xMilli:Int;

	/** Vertical placement in thousandths of one world block. */
	final yMilli:Int;

	/** Depth placement in thousandths of one world block. */
	final zMilli:Int;
}
