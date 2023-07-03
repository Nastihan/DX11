#include "Material.h"

Material::Material(Graphics& gfx, const aiMaterial& material, const std::filesystem::path& path) noxnd
	:
modelPath(path.string())
{
	using namespace Bind;
	const auto rootPath = path.parent_path().string() + "\\";
	{
		aiString tempName;
		material.Get(AI_MATKEY_NAME, tempName);
		name = tempName.C_Str();
	}
	// phong technique
	{
		Technique phong{ "Phong" };
		Step step(0);
		std::string shaderCode = "Phong";
		aiString texFileName;

		// common (pre)
		vtxLayout.Append(Dvtx::VertexLayout::Position3D);
		vtxLayout.Append(Dvtx::VertexLayout::Normal);
		Dcb::RawLayout pscLayout;
		bool hasTexture = false;
		bool hasGlossAlpha = false;

		// diffuse
		{
			bool hasAlpha = false;
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Dif";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str());
				if (tex->HasAlpha())
				{
					hasAlpha = true;
					shaderCode += "Msk";
				}
				step.AddBindable(std::move(tex));
			}
			else
			{
				pscLayout.Add<Dcb::Float3>("materialColor");
			}
			step.AddBindable(Rasterizer::Resolve(gfx, hasAlpha));
		}
		// specular
		{
			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Spc";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 1);
				hasGlossAlpha = tex->HasAlpha();
				step.AddBindable(std::move(tex));
				pscLayout.Add<Dcb::Bool>("useGlossAlpha");
			}
			pscLayout.Add<Dcb::Float3>("specularColor");
			pscLayout.Add<Dcb::Float>("specularWeight");
			pscLayout.Add<Dcb::Float>("specularGloss");
		}
		// normal
		{
			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Nrm";
				vtxLayout.Append(Dvtx::VertexLayout::Texture2D);
				vtxLayout.Append(Dvtx::VertexLayout::Tangent);
				vtxLayout.Append(Dvtx::VertexLayout::Bitangent);
				step.AddBindable(Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 2));
				pscLayout.Add<Dcb::Bool>("useNormalMap");
				pscLayout.Add<Dcb::Float>("normalMapWeight");
			}
		}
		// common (post)
		{
			step.AddBindable(std::make_shared<TransformCbuf>(gfx, 0u));
			step.AddBindable(Blender::Resolve(gfx, false));
			auto pvs = VertexShader::Resolve(gfx, shaderCode + "VS.cso");
			auto pvsbc = pvs->GetBytecode();
			step.AddBindable(std::move(pvs));
			step.AddBindable(PixelShader::Resolve(gfx, shaderCode + "PS.cso"));
			step.AddBindable(InputLayout::Resolve(gfx, vtxLayout, pvsbc));
			if (hasTexture)
			{
				step.AddBindable(Bind::Sampler::Resolve(gfx));
			}
			// PS material params (cbuf)
			Dcb::Buffer buf{ std::move(pscLayout) };
			if (auto r = buf["materialColor"]; r.Exists())
			{
				aiColor3D color = { 0.45f,0.45f,0.85f };
				material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
				r = reinterpret_cast<DirectX::XMFLOAT3&>(color);
			}
			buf["useGlossAlpha"].SetIfExists(hasGlossAlpha);
			if (auto r = buf["specularColor"]; r.Exists())
			{
				aiColor3D color = { 0.18f,0.18f,0.18f };
				material.Get(AI_MATKEY_COLOR_SPECULAR, color);
				r = reinterpret_cast<DirectX::XMFLOAT3&>(color);
			}
			buf["specularWeight"].SetIfExists(1.0f);
			if (auto r = buf["specularGloss"]; r.Exists())
			{
				float gloss = 8.0f;
				material.Get(AI_MATKEY_SHININESS, gloss);
				r = gloss;
			}
			buf["useNormalMap"].SetIfExists(true);
			buf["normalMapWeight"].SetIfExists(1.0f);
			step.AddBindable(std::make_unique<Bind::CachingPixelConstantBufferEX>(gfx, std::move(buf), 1u));
		}
		phong.AddStep(std::move(step));
		techniques.push_back(std::move(phong));
	}
	// outline technique
	{
		Technique outline{"Outline"};
		// mask
		Step mask{ 1 };
		{
			auto pvs = VertexShader::Resolve(gfx, "SolidVS.cso");
			auto pvsbc = pvs->GetBytecode();
			mask.AddBindable(std::move(pvs));

			mask.AddBindable(InputLayout::Resolve(gfx, vtxLayout, pvsbc));

			mask.AddBindable(std::make_shared<TransformCbuf>(gfx));

			outline.AddStep(std::move(mask));
		}
		Step drawOutline{ 2 };
		{
			auto pvs = VertexShader::Resolve(gfx, "SolidVS.cso");
			auto pvsbc = pvs->GetBytecode();
			drawOutline.AddBindable(std::move(pvs));

			drawOutline.AddBindable(PixelShader::Resolve(gfx, "SolidPS.cso"));

			Dcb::RawLayout layout{};
			layout.Add<Dcb::Float4>("color");
			Dcb::Buffer buf{std::move(layout)};
			buf["color"] = DirectX::XMFLOAT4{ 1.0f,0.4f,0.4f,1.0f };
			drawOutline.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEX>(gfx, buf, 1u));

			drawOutline.AddBindable(InputLayout::Resolve(gfx, vtxLayout, pvsbc));

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
				std::unique_ptr<CloningBindable> Clone() const noexcept override
				{
					return std::make_unique<TransformCbufScaling>(*this);
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
			drawOutline.AddBindable(std::make_shared<TransformCbufScaling>(gfx));
			outline.AddStep(std::move(drawOutline));
		}
		techniques.push_back(outline);

	}
}
Dvtx::VertexBuffer Material::ExtractVertices(const aiMesh& mesh) const noexcept
{
	return { vtxLayout,mesh };
}
std::vector<Technique> Material::GetTechniques() const noexcept
{
	return techniques;
}