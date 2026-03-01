import labster.core: Rect, Sizes, Vector2
import labster.render: Renderer, RenderContext

class Placeholder {
	private final width: UInt32;
	private final height: UInt32;

	// Dart-like automatic arguments assignment
	constructor(this.width, this.height) {
		assert(this.width > 0)
		assert(this.height > 0)
	}

	implements Renderer {
		fun render(context: RenderContext): Size {
			context.drawRect(Rect(0, 0, this.width, this.height))
			context.drawLine(Vector2(0, 0), Vector2(this.width, this.height))
			context.drawLine(Vector2(this.width, 0), Vector2(0, this.height))
			return Sizes(Vector2(this.width, this.height))
		}
	}
}

// document is an object that exists in .lsm files only
document.renderElement(Placeholder(width, height))
