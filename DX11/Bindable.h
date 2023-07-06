#pragma once
#include "Graphics.h"
#include <memory>
#include "GraphicsResource.h"

class Drawable;
class TechniqueProbe;
namespace Bind
{
	class Bindable : public GraphicsResource
	{
	public:
		virtual void Bind(Graphics& gfx) noexcept = 0;
		virtual void InitializeParentReference(const Drawable&) noexcept
		{}
		virtual std::string GetUID() const noexcept
		{
			assert(false);
			return "";
		}
		virtual void Accept(TechniqueProbe& probe) 
		{
			// pretty much most of the bindable do not do anything when they accept a probe
		}
		virtual ~Bindable() = default;
	};

	class CloningBindable : public Bindable
	{
	public:
		virtual std::unique_ptr<CloningBindable> Clone() const noexcept = 0;
	};
}