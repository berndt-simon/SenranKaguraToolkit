#pragma once

#include "Exporter_Base.h"
#include <iosfwd>
#include <ostream>

class ColladaExporter :  public Exporter {
public:
	ColladaExporter();
	~ColladaExporter() = default;
	
	void save(const TMD::PP::Data_t& data) override;

	boost::filesystem::path& dae_suffix();
	const boost::filesystem::path& dae_suffix() const;

private:
	struct LeftPad_t {
		uint8_t level;

		LeftPad_t& operator++();
		LeftPad_t& operator--();
	};

	struct Block_t {
		Block_t(LeftPad_t& padding);
		~Block_t();

		LeftPad_t& _padding;
	};

	class PaddedOstream {
	public:
		PaddedOstream(std::ostream& out, LeftPad_t& padding);
		~PaddedOstream() = default;

		template<typename t>
		std::ostream& operator<<(const t& val) {
			for (auto i(0U); i < _padding.level; ++i) {
				_out << "\t";
			}
			return _out;
		}

		operator std::ostream&();
		operator const std::ostream&() const;

		const PaddedOstream& operator<<(std::ostream& (*func)(std::ostream&)) const;

		LeftPad_t& padding();
		const LeftPad_t& padding() const;

	private:
		LeftPad_t& _padding;
		std::ostream& _out;

	};
	
private:
	void write_document(PaddedOstream& out, const TMD::PP::Data_t& data);
	void write_material_images(PaddedOstream& out, const TMD::PP::Data_t& data);

	void write_geometry(PaddedOstream& out, const TMD::PP::Data_t& data);
	void write_geometry_position(PaddedOstream& out, const TMD::PP::Data_t& data);
	void write_geometry_normal(PaddedOstream& out, const TMD::PP::Data_t& data);
	void write_geometry_uv(PaddedOstream& out, const TMD::PP::Data_t& data);

	void write_geometry_face(PaddedOstream& out, const std::array<uint16_t, 3>& face);
	
	void write_geometry_accessor_3d(PaddedOstream& out, const std::string& source_id, uint32_t count);
	void write_geometry_accessor_uv(PaddedOstream& out, const std::string& source_id, uint32_t count);

	boost::filesystem::path _dae_suffix;


};

