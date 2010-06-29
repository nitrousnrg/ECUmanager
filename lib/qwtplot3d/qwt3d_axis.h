#ifndef __AXIS_H__
#define __AXIS_H__

#include "qwt3d_autoptr.h"
#include "qwt3d_label.h"
#include "qwt3d_scale.h"
#include "qwt3d_autoscaler.h"

namespace Qwt3D
{

	//! Autoscalable axis with caption.
	/*!
	  Axes are highly customizable especially in terms
	  of labeling and scaling.
	*/
	class QWT3D_EXPORT Axis : public Drawable
	{

		public:

			Axis();				 //!< Constructs standard axis
								 //!< Constructs a new axis with specified limits
			Axis(Qwt3D::Triple beg, Qwt3D::Triple end);
			virtual ~Axis();	 // dtor

			virtual void draw(); //!< Draws axis

								 //!< Positionate axis
			void setPosition(const Qwt3D::Triple& beg, const Qwt3D::Triple& end);
								 //!< Returns axis' position
			void position(Qwt3D::Triple& beg, Qwt3D::Triple& end) const
			{
				beg = beg_; end = end_;
			}
								 //!< Returns axis' beginning position
			Qwt3D::Triple begin() const
			{
				return beg_;
			}
								 //!< Returns axis' ending position
			Qwt3D::Triple end() const
			{
				return end_;
			}
			double length() const//!< Returns axis' length
			{
				return (end_-beg_).length();
			}

								 //!< Sets tics lengths in world coordinates
			void setTicLength(double majorl, double minorl);
			//! Returns tics lengths
			void ticLength(double& majorl, double& minorl) const {majorl = lmaj_; minorl = lmin_;}
								 //!< Sets tic orientation
			void setTicOrientation(double tx, double ty, double tz);
								 //!< Same function as above
			void setTicOrientation(const Qwt3D::Triple& val);
								 //!< Returns tic orientation
			Qwt3D::Triple ticOrientation() const
			{
				return orientation_;
			}
								 //!< Sets two-sided tics (default is false)
			void setSymmetricTics( bool b)
			{
				symtics_ = b;
			}

			//! Sets font for axis label
			void setLabelFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
								 //!< Sets font for axis label
			void setLabelFont(QFont const& font);
								 //!< Returns current label font
			QFont const& labelFont() const
			{
				return labelfont_;
			}

								 //!< Sets label content
			void setLabelString(QString const& name);
			void setLabelPosition(const Qwt3D::Triple& pos, Qwt3D::ANCHOR);
			void setLabelColor(Qwt3D::RGBA col);
			void setLabel(bool d)//!< Turns label drawing on or off
			{
				drawLabel_ = d;
			}
								 //!< Shifts label in device coordinates dependent on anchor;
			void adjustLabel(int val)
			{
				labelgap_ = val;
			}

								 //!< Turns scale drawing on or off
			void setScaling(bool d)
			{
				drawTics_ = d;
			}
			bool scaling() const //!< Returns, if scale drawing is on or off
			{
				return drawTics_;
			}
			void setScale(Qwt3D::SCALETYPE);
			void setScale(Scale* item);
								 //!< Turns number drawing on or off
			void setNumbers(bool d)
			{
				drawNumbers_ = d;
			}
			bool numbers() const //!< Returns, if number drawing is on or off
			{
				return drawNumbers_;
			}
								 //!< Sets the color for axes numbers
			void setNumberColor(Qwt3D::RGBA col);
								 //!< Returns the color for axes numbers
			Qwt3D::RGBA numberColor() const
			{
				return numbercolor_;
			}
			//! Sets font for numbering
			void setNumberFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
								 //!< Overloaded member, works like the above function
			void setNumberFont(QFont const&);
								 //!< Returns current numbering font
			QFont const& numberFont() const
			{
				return numberfont_;
			}
								 //!< Sets anchor position for numbers
			void setNumberAnchor(Qwt3D::ANCHOR a)
			{
				scaleNumberAnchor_ = a;
			}
								 //!< Shifts axis numbers in device coordinates dependent on anchor;
			void adjustNumbers(int val)
			{
				numbergap_ = val;
			}

								 //!< Turns Autoscaling on or off
			void setAutoScale(bool val = true)
			{
				autoscale_ = val;
			}
								 //!< actual Autoscaling mode
			bool autoScale() const
			{
				return autoscale_;
			}

								 //!< Requests major intervals (maybe changed, if autoscaling is present)
			void setMajors(int val);
								 //!< Requests minor intervals
			void setMinors(int val);
			int majors() const	 //!< Returns number of major intervals
			{
				return majorintervals_;
			}
			int minors() const	 //!< Returns number of minor intervals
			{
				return minorintervals_;
			}
								 //!< Returns positions for actual major tics (also if invisible)
			Qwt3D::TripleField const& majorPositions() const
			{
				return majorpos_;
			}
								 //!< Returns positions for actual minor tics (also if invisible)
			Qwt3D::TripleField const& minorPositions() const
			{
				return minorpos_;
			}

			//! Sets line width for axis components
			void setLineWidth(double val, double majfac = 0.9, double minfac = 0.5);
								 //!< Returns line width for axis body
			double lineWidth() const
			{
				return lineWidth_;
			}
								 //!< Returns Line width for major tics
			double majLineWidth() const
			{
				return majLineWidth_;
			}
								 //!< Returns Line width for minor tics
			double minLineWidth() const
			{
				return minLineWidth_;
			}

								 //!< Sets interval
			void setLimits(double start, double stop)
			{
				start_=start; stop_=stop;
			}
								 //!< Returns axis interval
			void limits(double& start, double& stop) const
			{
				start = start_; stop = stop_;
			}
								 //!< Enforces recalculation of ticmark positions
			void recalculateTics();

		private:

			void init();
			void drawBase();
			void drawTics();
			void drawTicLabel(Qwt3D::Triple Pos, int mtic);
			Qwt3D::Triple drawTic(Qwt3D::Triple nadir, double length);
			void drawLabel();
			bool prepTicCalculation(Triple& startpoint);

			Qwt3D::Triple biggestNumberString();

			Qwt3D::ANCHOR scaleNumberAnchor_;
			Qwt3D::Label label_;
			std::vector<Qwt3D::Label> markerLabel_;

			Qwt3D::Triple beg_, end_;
								 //! vectors, holding major resp. minor tic positions;
			Qwt3D::TripleField majorpos_, minorpos_;

								 //!< enclosing parallelepiped for axis numbering
			Qwt3D::Triple ncube_beg_, ncube_end_;

			double start_, stop_, autostart_, autostop_;
			double lmaj_, lmin_;
			Qwt3D::Triple orientation_;

			int majorintervals_, minorintervals_;

			double lineWidth_, majLineWidth_, minLineWidth_;
			bool symtics_;
			bool drawNumbers_, drawTics_, drawLabel_;
			bool autoscale_;
			QFont numberfont_, labelfont_;
			Qwt3D::RGBA  numbercolor_;

			int numbergap_, labelgap_;

			Qwt3D::qwt3d_ptr<Qwt3D::Scale> scale_;
	};

}								 // ns
#endif
