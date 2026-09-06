class AlgoVizError(Exception):
    """Base class for user-facing AlgoViz failures."""


class IntegrityError(AlgoVizError):
    """Raised when an input changes during a run."""


class SymbolResolutionError(AlgoVizError):
    """Raised when a requested C++ symbol cannot be resolved uniquely."""


class WorkspaceError(AlgoVizError):
    """Raised when a safe external workspace cannot be created."""
