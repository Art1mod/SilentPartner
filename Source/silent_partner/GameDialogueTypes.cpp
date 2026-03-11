#include "GameDialogueTypes.h"
#include "DialogueDataRow.h"


FText UDialogueNode::GetNodeText() const
{
    // Now the compiler knows what FDialogueDataRow is
    if (FDialogueDataRow* Row = DialogueRow.GetRow<FDialogueDataRow>(TEXT("DialogueContext")))
    {
        return Row->DialogueText;
    }
    return FText::FromString("Error: Row Not Found");
}