#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H


namespace oscv
{
/**
 * @brief The IProgressBar class is an abstract class for the interface of progress dialog
 *        for both GUI and console
 */
class IProgressBar
{

public:
    virtual ~IProgressBar() {}

    /**
     * @brief setModal set progress dialog modality. This is applied also for the console version.
     * @param modal true if the dialog is modal and false for modeless dialog
     */
    virtual void setModal(bool modal=true) = 0;

    /**
     * @brief setValue loop function in the caller call this function to set the progress
     * @param progress progress to be set in the loop @see setMaximum();
     */
    virtual void setValue(int progress) = 0;

    /**
     * @brief wasCanceled this function is called in the busy loop to check if the cancel has be set.
     * @return true if the cancel is called during processing in the loop.
     */
    virtual bool wasCanceled() const = 0;

    /**
     * @brief setMaximum set maximum value if the range (of the loop) is not between 0 and 100
     * @param maximum
     */
    virtual void setMaximum(int maximum) = 0;

};
}
#endif // PROGRESSBAR_H
