#ifndef PHSTRIPDOC_H
#define PHSTRIPDOC_H

class PhStripDoc {

public:

    PhStripDoc();

    /**
     * Describes the type of the cut.
     */
    enum PhCutType {
        Simple, FadeIn, FadeOut, CrossFade
    };


    /**
     * A cut is a change in the video from one shot to another. It can be simple (one frame change) or progressive (fade).
     */
    class PhStripCut : PhStripDoc::PhStripObject {

    private:
        PhStripDoc::PhCutType type;
    };



    /**
     * Sentence or part of a sentence of a PhPeople with synchronization information.
     */
    class PhStripText : PhStripDoc::PhStripObject {

    private:
        /**
         * Indicate the end of the PhText. The text content is stretch between timeIn : PhTime and timeOut : PhTime
         */
        PhTime timeOut;
        /**
         * Text string of Sentence or part of a sentence.
         */
        PhString content;
    };



    /**
     * Indicate the start of a new loop. A loop is in fact a section of the rythmo strip. The detection process consists to cut the video content into loops to simplify the convocation of the dubbing actors for recording. It is generally between 30 seconds and 1 minute long: what the actor can record in one take.
     */
    class PhStripLoop : PhStripDoc::PhStripObject {

    private:
        /**
         * Number of the loop. The first loop can be 1 or another value. The loop numbering is usually continuous but not always.
         */
        int number;
    };
    /**
     * Generic object of a rythmo strip (loop, cut, text, ...).
     */



    class PhStripObject {

    private:
        /**
         * Starting time of the object.
         */
        PhTime timeIn;
    };




    /**
     * Represents a character or a group of character from the video content. It can also be use to write notes or comments on the strip. For example : NDA (note de l'adaptateur).
     */
    class PhPeople {

    private:
        /**
         * Name of the people
         */
        PhString name;
        /**
         * Color of the people's text on the strip.
         */
        PhColor color;

    public:
        PhPeople();
        PhString getName();
        void setName(PhString name);
        PhColor getColor();
        void setColor(PhColor color);
    };




    /**
     * Modelize the content of a rythmo strip.
     */
    class PhStripDoc {

    private:
        /**
         * Title of the corresponding audiovisual content.
         */
        int title;
        /**
         * Starting time of the video content refered by the videoPath : String
         */
        PhTimeCode videoTimestamp;
        /**
         * Path to the video content.
         */
        PhString videoPath;
        /**
         * Amount of time units per second.
         */
        int timeScale;
        /**
         * Number of video frame per second. In fact, this attribute can take 4 differents values:
         * 23.98
         * 24
         * 25
         * 29.97
         */
        float fps;
        /**
         * Specify if the timecode use dropframe or not. In fact, only 29.97 video framerate use dropframe.
         */
        bool drop;

    public:
        PhStripDoc();
        void getTitle();
        void setTitle(int title);
        PhTimeCode getVideoTimestamp();
        void setVideoTimestamp(PhTimeCode videoTimestamp);
        String getVideoPath();
        void setVideoPath(PhString videoPath);
        int getTimeScale();
        void setTimeScale(int timeScale);
    };
};
#endif // PHSTRIPDOC_H
