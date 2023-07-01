#include "TestCube.h"
#include "Cube.h"
#include "BindableCommon.h"
#include "TransformCbufPS.h"
#include "imgui/imgui.h"
#include "Stencil.h"
#include "DynamicConstant.h"
#include "ConstantBuffersEx.h"
#include "TechniqueProbe.h"

TestCube::TestCube(Graphics& gfx, float size)
{
	auto model = Cube::MakeIndependentTextured();
	model.Transform(DirectX::XMMatrixScaling(size, size, size));
	model.SetNormalsIndependentFlat();
	const auto geometryTag = "$cube." + std::to_string(size);

	using namespace Bind;
	pTopology = Topology::Resolve(gfx, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pIndices = IndexBuffer::Resolve(gfx, geometryTag, model.indices);
	pVertices = VertexBuffer::Resolve(gfx, geometryTag, model.vertices);

	// lambertian
	{
		Technique lambertian{"Lambertian"};
		{
			Step first{ 0 };

			first.AddBindable(Texture::Resolve(gfx, "Images\\brickwall.jpg"));
			first.AddBindable(Sampler::Resolve(gfx));

			auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
			auto pvsbc = pvs->GetBytecode();
			first.AddBindable(std::move(pvs));

			first.AddBindable(PixelShader::Resolve(gfx, "PhongPS.cso"));

			Dcb::RawLayout layout;
			layout.Add<Dcb::Float>("specularIntensity");
			layout.Add<Dcb::Float>("specularPower");
			layout.Add<Dcb::Bool>("normalMappingEnabled");
			Dcb::Buffer buf(std::move(layout));
			buf["specularIntensity"] = 0.1f;
			buf["specularPower"] = 20.0f;
			first.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEX>(gfx, buf, 1u));

			first.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

			auto tcb = std::make_shared<TransformCbuf>(gfx, 0u);
			first.AddBindable(tcb);

			first.AddBindable(Rasterizer::Resolve(gfx, false));

			first.AddBindable(Blender::Resolve(gfx, false));

			lambertian.AddStep(std::move(first));
		}
		AddTechnique(lambertian);
	}
	// outlining
	{
		Technique outline{"Outline"};
		{
			Step mask{ 1 };
			{
				mask.AddBindable(Texture::Resolve(gfx, "Images\\brickwall.jpg"));
				mask.AddBindable(Sampler::Resolve(gfx));

				auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
				auto pvsbc = pvs->GetBytecode();
				mask.AddBindable(std::move(pvs));

				mask.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

				auto tcb = std::make_shared<TransformCbuf>(gfx, 0u);
				mask.AddBindable(tcb);

				mask.AddBindable(Rasterizer::Resolve(gfx, false));

				mask.AddBindable(Blender::Resolve(gfx, false));
			}
			outline.AddStep(mask);

			Step draw{ 2 };
			{
				auto pvs = VertexShader::Resolve(gfx, "SolidVS.cso");
				auto pvsbc = pvs->GetBytecode();
				draw.AddBindable(std::move(pvs));

				draw.AddBindable(PixelShader::Resolve(gfx, "SolidPS.cso"));

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float4>("color");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["color"] = DirectX::XMFLOAT4{ 1.0f,0.4f,0.4f,1.0f };
				draw.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEX>(gfx, buf, 1u));

				draw.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

				class TransformCbufScaling : public TransformCbuf
				{
				public:
					TransformCbufScaling(Graphics& gfx, float scale = 1.04)
						:
						TransformCbuf(gfx),
						buf(MakeLayout())
					{
						buf["scale"] = scale;
					}
					void Accept(TechniqueProbe& probe) override
					{
						probe.VisitBuffer(buf);
					}
					void Bind(Graphics& gfx) noexcept override
					{
						const float scale = buf["scale"];
						const auto scaleMatrix = DirectX::XMMatrixScaling(scale, scale, scale);
						auto xf = GetTransforms(gfx);
						xf.modelView = xf.modelView * scaleMatrix;
						xf.modelViewProj = xf.modelViewProj * scaleMatrix;
						UpdateBindImpl(gfx, xf);
					}
				private:
					static Dcb::RawLayout MakeLayout()
					{
						Dcb::RawLayout layout;
						layout.Add<Dcb::Float>("scale");
						return layout;
					}
				private:
					Dcb::Buffer buf;
				};
				draw.AddBindable(std::make_shared<TransformCbufScaling>(gfx));

				draw.AddBindable(Rasterizer::Resolve(gfx, false));

				draw.AddBindable(Blender::Resolve(gfx, false));
			}
			outline.AddStep(draw);
		}
		AddTechnique(outline);
	}

}

void TestCube::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

void TestCube::SetRotation(float roll, float pitch, float yaw) noexcept
{
	this->roll = roll;
	this->pitch = pitch;
	this->yaw = yaw;
}

DirectX::XMMATRIX TestCube::GetTransformXM() const noexcept
{
	auto xf = DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	if (outlining)
	{
		xf = DirectX::XMMatrixScaling(1.03f, 1.03f, 1.03f) * xf;
	}
	return xf;
}

void TestCube::SpawnControlWindow(Graphics& gfx, const char* name) noexcept
{

	if (ImGui::Begin(name))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

		class Probe : public TechniqueProbe
		{
		public:
			void OnSetTechnique() override
			{
				using namespace std::string_literals;
				ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, pTech->GetName().c_str());
				bool active = pTech->IsActive();
				ImGui::Checkbox(("Tech Active##"s + std::to_string(techIdx)).c_str(), &active);
				pTech->SetActiveState(active);
			}
			bool OnVisitBuffer(Dcb::Buffer& buf) override
			{
				namespace dx = DirectX;
				float dirty = false;
				const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
				auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string(bufIdx)]
				(const char* label) mutable
				{
					tagScratch = label + tagString;
					return tagScratch.c_str();
				};

				if (auto v = buf["scale"]; v.Exists())
				{
					dcheck(ImGui::SliderFloat(tag("Scale"), &v, 1.0f, 2.0f, "%.3f", 3.5f));
				}
				if (auto v = buf["color"]; v.Exists())
				{
					dcheck(ImGui::ColorPicker4(tag("Color"), reinterpret_cast<float*>(&static_cast<dx::XMFLOAT4&>(v))));
				}
				if (auto v = buf["specularIntensity"]; v.Exists())
				{
					dcheck(ImGui::SliderFloat(tag("Spec. Intens."), &v, 0.0f, 1.0f));
				}
				if (auto v = buf["specularPower"]; v.Exists())
				{
					dcheck(ImGui::SliderFloat(tag("Glossiness"), &v, 1.0f, 100.0f, "%.1f", 1.5f));
				}
				return dirty;
			}
		} probe;

		Accept(probe);
	}
	ImGui::End();
}
