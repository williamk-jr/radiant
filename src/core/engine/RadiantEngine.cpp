#include "radiant/core/engine/RadiantEngine.h"

#include "radiant/core/engine/StyleSheetPropertyResolvers.h"
#include "radiant/core/engine/StyleSheetStandardFunctions.h"
#include "radiant/core/engine/font/Bitmap.h"
#include "radiant/core/engine/font/Font.h"
#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/core/render/TextureAtlas.h"
#include "radiant/core/render/Window.h"
#include "radiant/core/render/models/Quad2D.h"
#include "radiant/core/render/resources/shader/Texture.h"
#include "radiant/core/render/vulkan/pipeline/VulkanGraphicsPipelineBuilder.h"
#include "radiant/css/Parser.h"
#include "radiant/css/StyleSheetEntry.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	RadiantEngine::RadiantEngine(const std::string& title, uint32_t width, uint32_t height) {
		this->window      = std::make_unique<Window>(title, width, height);
		this->fontManager = std::make_unique<FontManager>();
		this->notoSans =
		    std::make_unique<Font>(this->fontManager->loadFont("/usr/share/fonts/truetype/noto/NotoSans-Regular.ttf"));

		for (int i = 8; i <= 16; i++) {
			notoSans->setPointSize(i);
			std::unique_ptr<RenderBatch> batch =
			    this->fontManager->compileStringGeometry(*this->notoSans, {0, 0, (float)0xFFFFFFFF, (float)0xFFFFFFFF},
			                                             "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!.?,");

			// Logger::info("Batch Font Size: " + std::to_string(i));
			// for (int j = 0; j < batch->instances.size(); j++) {
			//	Logger::info("\tInstance " + std::to_string(j) + ": ");
			//	Logger::info("\t\tPosition: " + std::to_string(batch->instances[j].position.x) + ", "
			//	             + std::to_string(batch->instances[j].position.y));
			//	Logger::info("\t\tSize: " + std::to_string(batch->instances[j].size.x) + ", "
			//	             + std::to_string(batch->instances[j].size.y));
			// }
		}
		this->notoSans->setPointSize(16);

		// Enable debug logs.
#ifndef NDEBUG
		this->renderer = std::make_unique<Renderer>(*this->window, true);
#else
		this->renderer = std::make_unique<Renderer>(*this->window, false);
#endif

		this->stylesheetParser = std::make_unique<StyleSheetParser::Parser>();
		this->registerProperties();
		this->registerFunctions();

		this->widgetManager = std::make_unique<WidgetManager>(*this->window, *this->stylesheetParser);

		this->vertexBuffer   = renderer->createVertexBuffer(2048);
		this->instanceBuffer = renderer->createInstanceBuffer(1024 * 1024);
		this->indexBuffer    = renderer->createIndexBuffer(2048);
		this->uniformBuffer  = renderer->createUniformBuffer(2048);

		// Init graphics pipeline.
		VkPipelineColorBlendAttachmentState attachmentState{};
		attachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		attachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		attachmentState.colorBlendOp        = VK_BLEND_OP_ADD;
		// attachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		// attachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		// attachmentState.alphaBlendOp = VK_BLEND_OP_SUBTRACT;
		attachmentState.colorWriteMask =
		    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		attachmentState.blendEnable = VK_TRUE;

		this->solidColorGraphicsPipeline = std::make_unique<VulkanPipeline>(
		    VulkanGraphicsPipelineBuilder(this->renderer->getDevice())
		        .withLayout(this->renderer->getDescriptorSetLayouts())
		        .withRenderingInfo({VK_FORMAT_B8G8R8A8_SRGB}, VK_FORMAT_UNDEFINED, VK_FORMAT_UNDEFINED)
		        .withVertexBindingDescription(sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX,
		                                      {
		                                          {VK_FORMAT_R32G32_SFLOAT, 0},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)},
		                                      })
		        .withVertexBindingDescription(sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE,
		                                      {
		                                          {VK_FORMAT_R32G32B32A32_SFLOAT, 0},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, position)},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, size)},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, uvMin)},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, uvMax)},
		                                      })
		        .withInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE)
		        .withShaderSlang("main", "./shaders/main.slang", VK_SHADER_STAGE_VERTEX_BIT)
		        .withMultisampleState(VK_SAMPLE_COUNT_1_BIT, {VK_FALSE, 0.0}, VK_FALSE, VK_FALSE)
		        .withRasterizationState(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE,
		                                {VK_FALSE}, 1.0, VK_FALSE)
		        .withShaderSlang("main", "./shaders/main.slang", VK_SHADER_STAGE_FRAGMENT_BIT)
		        .withColorBlendState({attachmentState}, nullptr)
		        .withDynamicState({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR})
		        .withViewportState(1, 1)
		        .build());

		this->alphaMapGraphicsPipeline = std::make_unique<VulkanPipeline>(
		    VulkanGraphicsPipelineBuilder(this->renderer->getDevice())
		        .withLayout(this->renderer->getDescriptorSetLayouts())
		        .withRenderingInfo({VK_FORMAT_B8G8R8A8_SRGB}, VK_FORMAT_UNDEFINED, VK_FORMAT_UNDEFINED)
		        .withVertexBindingDescription(sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX,
		                                      {
		                                          {VK_FORMAT_R32G32_SFLOAT, 0},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)},
		                                      })
		        .withVertexBindingDescription(sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE,
		                                      {
		                                          {VK_FORMAT_R32G32B32A32_SFLOAT, 0},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, position)},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, size)},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, uvMin)},
		                                          {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, uvMax)},
		                                      })
		        .withInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE)
		        .withShaderSlang("main", "./shaders/main.slang", VK_SHADER_STAGE_VERTEX_BIT)
		        .withMultisampleState(VK_SAMPLE_COUNT_1_BIT, {VK_FALSE, 0.0}, VK_FALSE, VK_FALSE)
		        .withRasterizationState(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE,
		                                {VK_FALSE}, 1.0, VK_FALSE)
		        .withShaderSlang("main", "./shaders/texture.slang", VK_SHADER_STAGE_FRAGMENT_BIT)
		        .withColorBlendState({attachmentState}, nullptr)
		        .withDynamicState({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR})
		        .withViewportState(1, 1)
		        .build());

		std::unordered_map<std::string, StyleSheetParser::StyleSheet> styleSheets =
		    this->stylesheetParser->getStyleSheets("./assets/test.css");

		Quad2D                quad;
		std::vector<Vertex>   verticies = quad.getVerticies();
		std::vector<uint16_t> indicies  = quad.getIndicies();

		this->vertexBuffer->append(verticies);
		this->indexBuffer->append(indicies);
	}

	void RadiantEngine::registerProperties() {
		this->stylesheetParser->registerProperty("top", {StyleSheetParser::ValueTypes::UNIT},
		                                         {{StyleSheetParser::Unit{0.0f, StyleSheetParser::UnitType::PIXEL}}},
		                                         PropertyResolvers::UNIT_LIST_RESOLVER);
		this->stylesheetParser->registerProperty("bottom", {StyleSheetParser::ValueTypes::UNIT},
		                                         {{StyleSheetParser::Unit{0.0f, StyleSheetParser::UnitType::PIXEL}}},
		                                         PropertyResolvers::UNIT_LIST_RESOLVER);
		this->stylesheetParser->registerProperty("left", {StyleSheetParser::ValueTypes::UNIT},
		                                         {{StyleSheetParser::Unit{0.0f, StyleSheetParser::UnitType::PIXEL}}},
		                                         PropertyResolvers::UNIT_LIST_RESOLVER);
		this->stylesheetParser->registerProperty("right", {StyleSheetParser::ValueTypes::UNIT},
		                                         {{StyleSheetParser::Unit{0.0f, StyleSheetParser::UnitType::PIXEL}}},
		                                         PropertyResolvers::UNIT_LIST_RESOLVER);
	}

	void RadiantEngine::registerFunctions() {
		this->stylesheetParser->registerFunction("rgb", &StyleSheetStandardFunctions::rgb);
	}

	RadiantEngine::~RadiantEngine() {
		renderer->waitIdle();
	}

	void RadiantEngine::loadStylesheet(std::filesystem::path path) {}

	std::shared_ptr<Widget> RadiantEngine::getRootWidget() {
		return this->widgetManager->getRootWidget();
	}

	bool RadiantEngine::isRunning() {
		return !this->window->shouldClose();
	}

	void RadiantEngine::update() {
		if (this->fontManager->isTextureAtlasDirty()) {
			TextureAtlas& textureAtlas = this->fontManager->getTextureAtlas();
			this->fontAtlasGpu = this->renderer->createTexture(textureAtlas.getBuffer(), textureAtlas.getWidth(),
			                                                   textureAtlas.getHeight(), textureAtlas.getPixelSize());
			this->fontManager->markTextureAtlasClean();
		}

		RenderBatch batch = this->widgetManager->createRenderBatch();
		if (!batch.instances.empty()) { // If empty, instanceBuffer does not need to be updated
			this->instanceBuffer->resetOffset();
			this->instanceBuffer->append(batch.instances);
		}

		Radiant::Color color{0, 0, 0, 1};
		renderer->beginFrame(*window);
		renderer->beginRendering(color);

		renderer->bindPipeline(*this->solidColorGraphicsPipeline);

		Radiant::Rect2D frameBufferSize = window->getFrameBufferSize();
		renderer->setViewport(frameBufferSize.width, frameBufferSize.height, 0, 1.0);
		renderer->setScissor(frameBufferSize.width, frameBufferSize.height);

		// Update uniforms
		glm::mat4 orthoMatrix =
		    glm::ortho(0.0f, (float)frameBufferSize.width, 0.0f, (float)frameBufferSize.height, -1.0f, 1.0f);

		this->uniformBuffer->reset();
		this->uniformBuffer->append(&orthoMatrix, sizeof(glm::mat4));
		this->uniformBuffer->write();

		renderer->bindResource(*this->solidColorGraphicsPipeline, *this->uniformBuffer, 0);

		// Bind Font Atlas
		renderer->bindResource(*this->solidColorGraphicsPipeline, *this->fontAtlasGpu, 1);

		// Bind buffers
		renderer->bindVertexBuffer(*this->vertexBuffer);
		renderer->bindInstanceBuffer(*this->instanceBuffer, sizeof(Instance) * batch.instances.size());
		renderer->bindIndexBuffer(*this->indexBuffer);

		// Draw
		renderer->drawIndexed(6, batch.instances.size());

		std::unique_ptr<RenderBatch> charBatch = this->fontManager->compileStringGeometry(
		    *this->notoSans, {0, 0, 1000, 1000},
		    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam a libero vitae mauris accumsan hendrerit "
		    "et at risus. Nam non ultricies massa. Nulla finibus velit sit amet est lobortis, nec finibus leo dictum. "
		    "Nunc ut consequat lectus. Curabitur finibus augue magna, ut ornare nisl tristique vitae. Aenean sit amet "
		    "orci mi. Donec vitae dui cursus, ultrices purus nec, laoreet turpis.\n\n"

		    "In finibus sodales mauris, nec commodo neque porta sit amet. Donec lobortis elit nec dignissim dictum. "
		    "Pellentesque nisl mauris, porttitor nec ipsum eget, fermentum placerat eros. Curabitur ac sollicitudin "
		    "sem. Integer in tristique arcu. Nunc massa magna, mollis id nibh id, tristique semper magna. Cras "
		    "pulvinar lobortis tempor. Donec consequat placerat malesuada. Duis sit amet libero et erat fringilla "
		    "suscipit vel ac ex. Mauris dignissim volutpat porta.\n\n"

		    "Donec a egestas diam, non porttitor ante. Sed id malesuada lacus, eget imperdiet sem. Mauris nec magna "
		    "non mauris posuere finibus eget non enim. Fusce consectetur dolor id quam interdum auctor. Vestibulum "
		    "egestas condimentum diam, at posuere lacus ullamcorper dapibus. Sed efficitur gravida scelerisque. Ut "
		    "luctus, turpis ut cursus commodo, nibh nisl eleifend tellus, vitae vestibulum arcu mauris in quam. "
		    "Suspendisse ultricies feugiat massa. Curabitur mauris dolor, varius sit amet quam a, accumsan scelerisque "
		    "eros. Nunc ipsum enim, suscipit nec dui at, viverra condimentum nunc. Mauris vel posuere nisl, eget "
		    "pulvinar sapien.\n\n"

		    "Quisque justo felis, porta at odio gravida, fringilla euismod purus. Phasellus vel purus ac nibh volutpat "
		    "tincidunt in vel elit. Fusce fringilla gravida ex vel tincidunt. Ut congue ornare orci, nec consequat "
		    "magna viverra id. Vestibulum sed ante eu sem facilisis eleifend ut eget nibh. Curabitur in erat magna. In "
		    "imperdiet tempor libero aliquet vestibulum. Phasellus dapibus volutpat ligula vel commodo. Fusce faucibus "
		    "fermentum metus ac pulvinar. Curabitur vel metus fringilla, pharetra turpis et, faucibus nibh. Etiam sit "
		    "amet massa sed mi blandit tincidunt eget at enim. Nunc justo odio, hendrerit et est nec, ornare sodales "
		    "nulla.\n\n"

		    "Phasellus ac arcu nec nulla egestas interdum. Aenean vulputate nulla sed rutrum viverra. Proin cursus "
		    "tellus id sem rutrum luctus eget ac purus. Proin efficitur iaculis auctor. Suspendisse potenti. Cras "
		    "imperdiet vitae diam quis lacinia. Fusce in est et sapien sodales porttitor a id massa. Suspendisse "
		    "potenti. Cras iaculis urna leo, mollis congue tortor faucibus eu. Praesent dictum arcu lectus, nec "
		    "placerat massa elementum a. Ut fermentum imperdiet rhoncus. Sed convallis id dolor vitae tincidunt. "
		    "Mauris ultrices malesuada diam a tristique. Duis vitae porta lorem. Suspendisse quis condimentum est, non "
		    "porta nisi. Donec in dolor ligula.\n\n");
		VkDeviceSize charInstanceStart = this->instanceBuffer->getOffset();

		if (!charBatch->instances.empty()) { // If empty, instanceBuffer does not need to be updated
			this->instanceBuffer->append(charBatch->instances);
		}

		renderer->bindPipeline(*this->alphaMapGraphicsPipeline);
		renderer->bindResource(*this->alphaMapGraphicsPipeline, *this->uniformBuffer, 0);
		renderer->bindResource(*this->alphaMapGraphicsPipeline, *this->fontAtlasGpu, 1);

		renderer->bindInstanceBuffer(*this->instanceBuffer, charInstanceStart,
		                             sizeof(Instance) * charBatch->instances.size());

		renderer->drawIndexed(6, charBatch->instances.size());

		renderer->endRendering();
		renderer->endFrame();

		renderer->submit();
		renderer->present(*window);

		window->pollEvents();
	}
} // namespace Radiant
