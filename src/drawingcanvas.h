#pragma once

#include <wx/wx.h>
#include <vector>

#include "path.h"

class DrawingCanvas : public wxWindow
{
public:
    DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    virtual ~DrawingCanvas() noexcept {}

    void ShowExportDialog();

    int currentWidth{1};
    wxColour currentColor{*wxBLACK};

    const std::vector<Path> &GetSquiggles() const { return squiggles; }

    void SetSquiggles(const std::vector<Path> &&squiggles)
    {
        this->squiggles = std::move(squiggles);
        Refresh();
    }

private:
    void OnPaint(wxPaintEvent &);
    void DrawOnContext(wxGraphicsContext *gc);

    void OnMouseDown(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnMouseUp(wxMouseEvent &);
    void OnMouseLeave(wxMouseEvent &);

    bool isDrawing{};
    std::vector<Path> squiggles;

    wxMenu contextMenu;
    void BuildContextMenu();
    void OnContextMenuEvent(wxContextMenuEvent &);
};