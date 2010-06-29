#ifndef qwt3d_SurfacePlot_h__2004_03_05_11_36_begin_guarded_code
#define qwt3d_SurfacePlot_h__2004_03_05_11_36_begin_guarded_code

#include "qwt3d_plot.h"

namespace Qwt3D
{
	//! A class representing  Surfaces
	/**
		A SurfacePlot ...

	*/
	class QWT3D_EXPORT SurfacePlot : public Plot3D
	{
		Q_OBJECT

			public:
		#if QT_VERSION < 0x040000
			SurfacePlot( QWidget* parent = 0, const char* name = 0 );
		#else
			SurfacePlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
		#endif
			~SurfacePlot();
			void updateNormals();//!< Recalculates surface normals;
								 //!< Returns data resolution (1 means all data)
			int resolution() const
			{
				return resolution_p;
			}
								 //!< Returns the number of mesh cells for the ORIGINAL data
			std::pair<int,int> facets() const;
			bool loadFromData(Qwt3D::Triple** data, unsigned int columns, unsigned int rows
				, bool uperiodic = false, bool vperiodic = false);
			bool loadFromData(double** data, unsigned int columns, unsigned int rows
				,double minx, double maxx, double miny, double maxy);
			bool loadFromData(Qwt3D::TripleField const& data, Qwt3D::CellField const& poly);

			//! Equivalent to loadFromData();
			/**
			\deprecated  Use loadFromData instead
			*/
			bool createDataRepresentation(Qwt3D::Triple** data, unsigned int columns, unsigned int rows
				, bool uperiodic = false, bool vperiodic = false)
			{
				return loadFromData(data, columns, rows, uperiodic, vperiodic);
			}
			//! Equivalent to loadFromData();
			/**
			\deprecated  Use loadFromData instead
			*/
			bool createDataRepresentation(double** data, unsigned int columns, unsigned int rows
				,double minx, double maxx, double miny, double maxy)
			{
				return loadFromData(data, columns, rows, minx, maxx, miny, maxy);
			}
			//! Equivalent to loadFromData();
			/**
			\deprecated  Use loadFromData instead
			*/
			bool createDataRepresentation(Qwt3D::TripleField const& data, Qwt3D::CellField const& poly)
			{
				return loadFromData(data, poly);
			}

								 //!< Return floor style
			Qwt3D::FLOORSTYLE floorStyle() const
			{
				return floorstyle_;
			}
								 //!< Sets floor style
			void  setFloorStyle( Qwt3D::FLOORSTYLE val )
			{
				floorstyle_ = val;
			}
								 //!< Draw normals to every vertex
			void showNormals(bool);
			bool normals() const //!< Returns \c true, if normal drawing is on
			{
				return datanormals_p;
			}

								 //!< Sets length of normals in percent per hull diagonale
			void setNormalLength(double val);
								 //!< Returns relative length of normals
			double normalLength() const
			{
				return normalLength_p;
			}
								 //!< Increases plotting quality of normal arrows
			void setNormalQuality(int val);
								 //!< Returns plotting quality of normal arrows
			int normalQuality() const
			{
				return normalQuality_p;
			}

			signals:
			void resolutionChanged(int);

		public slots:
			void setResolution( int );

		protected:
			bool datanormals_p;
			double normalLength_p;
			int normalQuality_p;

			virtual void calculateHull();
			virtual void createData();
			virtual void createEnrichment(Qwt3D::Enrichment& p);
			virtual void createFloorData();
			void createNormals();
			void createPoints();

			int resolution_p;

			void readIn(Qwt3D::GridData& gdata, Triple** data, unsigned int columns, unsigned int rows);
			void readIn(Qwt3D::GridData& gdata, double** data, unsigned int columns, unsigned int rows,
				double minx, double maxx, double miny, double maxy);
			void calcNormals(GridData& gdata);
			void sewPeriodic(GridData& gdata);

			//void calcLowResolution();
		private:

			void Data2Floor();
			void Isolines2Floor();

			Qwt3D::FLOORSTYLE floorstyle_;

			// grid plot

			Qwt3D::GridData* actualDataG_;
			virtual void createDataG();
			virtual void createFloorDataG();
			void createNormalsG();
			void Data2FloorG();
			void Isolines2FloorG();
			void setColorFromVertexG(int ix, int iy, bool skip = false);

			// mesh plot

			Qwt3D::CellData* actualDataC_;
			virtual void createDataC();
			virtual void createFloorDataC();
			void createNormalsC();
			void Data2FloorC();
			void Isolines2FloorC();
			void setColorFromVertexC(int node, bool skip = false);
	};

}								 // ns
#endif
